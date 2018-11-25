
// RhinoDevel, MT, 2018nov18

#include <stdint.h>
#include <assert.h>

#include "node.h"
#include "mem.h"

void node_fill(uint32_t const node_addr, struct node * const n)
{
    uint32_t addr = node_addr;

    n->last_node_addr = mem_load_dword(addr);
    addr += 4;

    assert(n->last_node_addr != NODE_FREE_FLAG_WORD);

    n->block_addr = mem_load_dword(addr);
    addr += 4;

    n->block_len = mem_load_dword(addr);
    addr += 4;

    n->is_allocated = mem_load_byte(addr);
    ++addr;

    n->next_node_addr = mem_load_dword(addr);
}

void node_store(uint32_t node_addr, struct node const * const n)
{
    uint32_t addr = node_addr;

    // Pointer to last free block node:

    mem_store_dword(addr, n->last_node_addr);
    addr += 4;

    // Start address of free block:

    mem_store_dword(addr, n->block_addr);
    addr += 4;

    // Length of free block:

    mem_store_dword(addr, n->block_len);
    addr += 4;

    // Is node allocated or free?

    mem_store_byte(addr, n->is_allocated);
    ++addr;

    // Pointer to next free block node:

    mem_store_dword(addr, n->next_node_addr);
}
