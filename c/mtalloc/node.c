
// RhinoDevel, MT, 2018nov18

#include <stdint.h>

#include "node.h"
#include "mem.h"

void node_fill(uint16_t const node_addr, struct node * const n)
{
    uint16_t addr = node_addr;

    n->last_node_addr = mem_load_word(addr);
    addr += 2;

    // Debug.Assert(retVal.LastNodeAddr != FreeFlagWord);

    n->block_addr = mem_load_word(addr);
    addr += 2;

    n->block_len = mem_load_word(addr);
    addr += 2;

    n->is_allocated = mem_load_byte(addr);
    ++addr;

    n->next_node_addr = mem_load_word(addr);
}

void node_store(uint16_t node_addr, struct node const * const n)
{
    uint16_t addr = node_addr;

    // Pointer to last free block node:

    mem_store_word(addr, n->last_node_addr);
    addr += 2;

    // Start address of free block:

    mem_store_word(addr, n->block_addr);
    addr += 2;

    // Length of free block:

    mem_store_word(addr, n->block_len);
    addr += 2;

    // Is node allocated or free?

    mem_store_byte(addr, n->is_allocated);
    ++addr;

    // Pointer to next free block node:

    mem_store_word(addr, n->next_node_addr);
}
