
// RhinoDevel, MT, 2018nov19

#include <stdint.h>

#include "node.h"
#include "nodemem.h"
#include "alloc.h"

void alloc_free(uint16_t const block_addr)
{
    uint16_t node_addr = 0;
    struct node cur;

    if(block_addr == 0)
    {
        // //Debug.Assert(false);
        return; // Nothing to do.
    }

    node_addr = nodemem_get_block_node_addr(block_addr);
    if(node_addr == 0)
    {
        // Debug.Assert(false);
        return; // No block at given (start) address is allocated.
    }

    node_fill(node_addr, &cur);

    if(cur.is_allocated == 0)
    {
        // Debug.Assert(false);
        return; // Already deallocated.
    }

    cur.is_allocated = 0;
    node_store(node_addr, &cur);

// #if DEBUG
//     Mem.Clear(cur.BlockAddr, cur.BlockLen, 0xCD);
// #endif //DEBUG

    if(cur.next_node_addr != 0)
    {
        nodemem_merge_unallocated_with_next_if_possible(node_addr);
    }
    if(cur.last_node_addr != 0)
    {
        struct node last;

        node_fill(cur.last_node_addr, &last);

        if(last.is_allocated == 0)
        {
            nodemem_merge_unallocated_with_next_if_possible(cur.last_node_addr);
        }
    }

    nodemem_limit_free_nodes();
}

uint16_t alloc_alloc(uint16_t const wanted_len)
{
    uint16_t new_node_addr = 0, node_addr = 0;
    struct node n, new_node;

    if(wanted_len == 0)
    {
        return 0;
    }

    // Make sure that there is a node space available, first
    // (and maybe frees space from first node, which must
    // be done BEFORE first node may gets selected as
    // "alloc" node):
    //
    if(!nodemem_try_to_reserve_node_space())
    {
        return 0; // No more space for another node available.
    }

    node_addr = nodemem_get_alloc_node_addr(wanted_len);
    if(node_addr == 0)
    {
        return 0;
    }

    node_fill(node_addr, &n);

    // Debug.Assert(n.is_allocated == 0);

    if(n.block_len == wanted_len)
    {
        n.is_allocated = 1;
        node_store(node_addr, &n);
        return n.block_addr;
    }

    new_node.block_len = wanted_len;
    new_node.is_allocated = 1;
    new_node.last_node_addr = node_addr;
    new_node.next_node_addr = n.next_node_addr;
    new_node.block_addr = (uint16_t)(n.block_addr + n.block_len - wanted_len);

    new_node_addr = nodemem_store(&new_node);
    if(new_node_addr == 0)
    {
        return 0;
    }
    node_fill(node_addr, &n);

    // Debug.Assert(n.block_len > new_node.block_len);

    n.block_len -= new_node.block_len;
    n.next_node_addr = new_node_addr;
    node_store(node_addr, &n);

    if (new_node.next_node_addr != 0)
    {
        struct node next_node;

        node_fill(new_node.next_node_addr, &next_node);

        next_node.last_node_addr = new_node_addr;

        node_store(new_node.next_node_addr, &next_node);
    }

    return new_node.block_addr;
}
