
// RhinoDevel, MT, 2018nov18

#include <stdint.h>

#include "mem.h"

uint16_t const g_mem_addr_first = 8; // Must not be 0?! Stupid..
uint16_t g_mem_heap_len = 0;

static uint8_t * s_heap = 0;
static uint16_t s_heap_len = 0;

void mem_store_word(uint16_t const addr, uint16_t const val)
{
    // MT_TODO: TEST: Implement!
}

uint16_t mem_load_word(uint16_t const addr)
{
    return 0; // MT_TODO: TEST: Implement!
}

void mem_store_byte(uint16_t const addr, uint8_t const val)
{
    // MT_TODO: TEST: Implement!
}

uint8_t mem_load_byte(uint16_t const addr)
{
    return 0; // MT_TODO: TEST: Implement!
}

void mem_init(uint8_t * const heap, uint16_t const heap_len)
{
    s_heap = heap;
    s_heap_len = heap_len;

    g_mem_heap_len = s_heap_len - g_mem_addr_first;
}
