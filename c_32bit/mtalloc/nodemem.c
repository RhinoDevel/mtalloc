
// RhinoDevel, MT, 2018nov17

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "nodemem.h"
#include "mem.h"
#include "node.h"

extern uint32_t g_mem_heap_len;
extern uint32_t g_mem_addr_first;

static uint32_t s_max_node_count = 0; // How many nodes can currently be stored.

static uint32_t s_first_node_addr = 0; // First node's address.

static uint32_t get_first_block_addr()
{
    assert(s_first_node_addr == g_mem_addr_first);
    assert(s_max_node_count > 0);

    // First block begins behind last node.

    return (uint32_t)(s_first_node_addr + NODE_LEN * s_max_node_count);
}

/** Mark node's space in memory at given position as free for reuse.
 */
static void mark_node_space_as_free(uint32_t const node_addr)
{
    assert(s_first_node_addr == g_mem_addr_first);
    assert(node_addr >= s_first_node_addr);
    assert(node_addr <= get_first_block_addr());
    assert((node_addr - s_first_node_addr) % NODE_LEN == 0);

    mem_store_dword(node_addr, NODE_FREE_FLAG_WORD);
}

static uint32_t get_free_node_addr()
{
    assert(s_max_node_count > 0);
    assert(s_first_node_addr == g_mem_addr_first);

    for(uint32_t i = 0;i < s_max_node_count; ++i)
    {
        uint32_t const addr = (uint32_t)(s_first_node_addr + i * NODE_LEN),
            first_word = mem_load_dword(addr);

        if (first_word == NODE_FREE_FLAG_WORD)
        {
            return addr; // Found free space for a node.
        }
    }
    return 0; // No space for a node found.
}

static uint32_t get_first_block_node_addr()
{
    return nodemem_get_block_node_addr(get_first_block_addr());
}

static uint32_t create_free_node_addr()
{
    assert(get_free_node_addr() == 0);

    uint32_t const first_block_node_addr = get_first_block_node_addr();
    struct node first_block_node;
    uint32_t free_node_addr = 0;

    node_fill(first_block_node_addr, &first_block_node);

    // Is first block's node unallocated?
    //
    if(first_block_node.is_allocated != 0)
    {
        return 0;
    }

    // Is first block's length equal or longer than a node's length
    // plus one (at least one byte must be on stack for new node to
    // be created for this to makes sense..)?
    //
    if(first_block_node.block_len < NODE_LEN + 1)
    {
        return 0;
    }

    free_node_addr = first_block_node.block_addr;
    first_block_node.block_addr += NODE_LEN;
    first_block_node.block_len -= NODE_LEN;
    node_store(first_block_node_addr, &first_block_node);
    ++s_max_node_count;
    mark_node_space_as_free(free_node_addr);

    assert(get_free_node_addr() == free_node_addr);
    return free_node_addr;
}

static uint32_t get_or_create_free_node_space()
{
    uint32_t const node_addr = get_free_node_addr();

    if(node_addr != 0)
    {
        return node_addr;
    }
    return create_free_node_addr();
}

uint32_t nodemem_get_block_node_addr(uint32_t const block_addr)
{
    assert(block_addr > 0);
    assert(s_max_node_count > 0);
    assert(s_first_node_addr == g_mem_addr_first);

    uint32_t ret_val = 0;

    for (uint32_t i = 0; i < s_max_node_count; ++i)
    {
        uint32_t const addr = (uint32_t)(s_first_node_addr + i * NODE_LEN),
            first_word = mem_load_dword(addr);
        struct node n;

        if (first_word == NODE_FREE_FLAG_WORD)
        {
            continue; // Free space for a node.
        }

        node_fill(addr, &n);
        if(n.block_addr == block_addr)
        {
            ret_val = addr;
            break;
        }
    }
    assert(ret_val > 0 && ret_val < 0xFFFFFFFF);//uint32.MaxValue);
    return ret_val;
}

bool nodemem_try_to_reserve_node_space()
{
    return get_or_create_free_node_space() != 0;
}

uint32_t nodemem_store(struct node const * const n)
{
    assert(n != 0);

    uint32_t const node_addr = get_or_create_free_node_space();

    if(node_addr == 0)
    {
        return 0;
    }

    node_store(node_addr, n);
    return node_addr;
}

uint32_t nodemem_get_alloc_node_addr(uint32_t const wanted_len)
{
    assert(wanted_len > 0);

    uint32_t ret_val = 0,
        block_len = 0;
    uint32_t const first_block_addr = get_first_block_addr();
    struct node n;

    for (uint32_t addr = s_first_node_addr;
        addr != 0;
        addr = n.next_node_addr)
    {
        node_fill(addr, &n);

        if (n.is_allocated != 0)
        {
            continue; // Node's block is already allocated.
        }

        if (n.block_len < wanted_len)
        {
            continue; // Would not fit in node's block.
        }

        if (n.block_addr == first_block_addr)
        {
            if (ret_val == 0)
            {
                ret_val = addr; // No better candidate found, yet.
                block_len = n.block_len;
            }
            continue;
        }

        if (n.block_len == wanted_len)
        {
            ret_val = addr;
            break; // Fits perfectly. Found!
        }

        if (ret_val == 0)
        {
            ret_val = addr;
            block_len = n.block_len;
            continue; // No better candidate found, yet.
        }

        if (block_len > n.block_len)
        {
            // Would fit better than current best candidate.

            ret_val = addr;
            block_len = n.block_len;
        }
    }
    return ret_val;
}

static uint32_t get_last_free_node_count()
{
    assert(mem_load_dword(s_first_node_addr) != NODE_FREE_FLAG_WORD);

    uint32_t retVal = 0xFFFFFFFF/*uint32.MaxValue*/,
        addr = get_first_block_addr();

    do
    {
        ++retVal;

        addr -= NODE_LEN;
    } while (mem_load_dword(addr) == NODE_FREE_FLAG_WORD);

    return retVal;
}

void nodemem_merge_unallocated_with_next_if_possible(
    uint32_t const unallocated_node_addr)
{
    struct node cur, next;

    node_fill(unallocated_node_addr, &cur);

    assert(cur.is_allocated == 0);

    if(cur.next_node_addr == 0)
    {
        return; // Nothing to do.
    }

    node_fill(cur.next_node_addr, &next);

    assert(cur.block_addr + cur.block_len == next.block_addr);

    if(next.is_allocated != 0)
    {
        return; // Not possible. Nothing to do.
    }

    if(next.next_node_addr != 0)
    {
        // Merge current with next node:

        struct node next_next_node;

        node_fill(next.next_node_addr, &next_next_node);

        assert(next_next_node.last_node_addr == cur.next_node_addr);
        assert(next_next_node.is_allocated == 1);
        assert(next.block_addr + next.block_len == next_next_node.block_addr);

        next_next_node.last_node_addr = unallocated_node_addr;

        node_store(next.next_node_addr, &next_next_node);
    }

    mark_node_space_as_free(cur.next_node_addr);

    cur.next_node_addr = next.next_node_addr;
    cur.block_len += next.block_len;

    node_store(unallocated_node_addr, &cur);
}

void nodemem_limit_free_nodes()
{
    // ??????????????????|FFFF??????????????|*
    // ^                  ^                  ^
    // |                  |                  |
    // s_first_node_addr     |                  first block's address
    // OR                 Last free node's address
    // last non-free
    // node's address

    struct node first_node;
    uint32_t c = 0;

    node_fill(s_first_node_addr, &first_node);

    if(first_node.is_allocated != 0)
    {
        // Debug.WriteLine(
        //     "NodeMem.LimitFreeNodes : Warning:"
        //     + " First node is allocated.");
        return;
    }

    c = get_last_free_node_count();
    if(c < 2)
    {
        return; // Nothing to do (keep one free node, if existing).
    }
    --c; // Keep a single free node.

    s_max_node_count -= c;
    first_node.block_len += (uint32_t)(c * NODE_LEN);
    first_node.block_addr = get_first_block_addr();

    assert(mem_load_dword(first_node.block_addr) == NODE_FREE_FLAG_WORD);

    node_store(s_first_node_addr, &first_node);

    assert(get_last_free_node_count() == 1);

#ifndef NDEBUG
     mem_clear(first_node.block_addr, first_node.block_len, 0xDE);
#endif //NDEBUG
}

void nodemem_init()
{
    assert(s_max_node_count == 0);
    assert(s_first_node_addr == 0);

    uint32_t first_block_addr = 0, first_block_len = 0;
    struct node first_node;

    s_max_node_count = 1;
    s_first_node_addr = g_mem_addr_first;

    // The first (and initially only) block will occupy the complete
    // last part of heap space after the first node's memory space:
    //
    first_block_addr = get_first_block_addr();
    first_block_len = g_mem_heap_len - NODE_LEN;

    first_node.last_node_addr = 0;
    first_node.block_addr = first_block_addr;
    first_node.block_len = first_block_len;
    first_node.is_allocated = 0;
    first_node.next_node_addr = 0;

    node_store(s_first_node_addr, &first_node);
}
