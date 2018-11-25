
// RhinoDevel, MT, 2018nov18

#include <stdint.h>

#ifndef NDEBUG
#include <assert.h>
#include <stdio.h>
#endif //NDEBUG

#include "mem.h"

uint16_t const g_mem_addr_first = 8; // Must not be 0?! Stupid..

static uint8_t * s_mem = 0;
static uint16_t s_mem_len = 0;

uint16_t g_mem_heap_len = 0;

static void fill(uint16_t val, uint8_t * const low, uint8_t * const high)
{
    *low = (uint8_t)(val & 0xFF);
    *high = (uint8_t)(val >> 8);
}

void mem_store_word(uint16_t const addr, uint16_t const val)
{
    fill(val, s_mem + addr, s_mem + addr + 1);
}

uint16_t mem_load_word(uint16_t const addr)
{
    return (uint16_t)(s_mem[addr] | (s_mem[addr + 1] << 8));
}

void mem_store_byte(uint16_t const addr, uint8_t const val)
{
    s_mem[addr] = val;
}

uint8_t mem_load_byte(uint16_t const addr)
{
    return s_mem[addr];
}

void mem_clear(uint16_t const addr, uint16_t const len, uint8_t const val)
{
    uint16_t const lim = addr + len;

    for(uint16_t cur = addr;cur < lim; ++cur)
    {
        mem_store_byte(cur, val);
    }
}

#ifndef NDEBUG
void mem_print()
{   
    static uint16_t const columns = 16;
    
    assert(columns < 256);

    for(uint16_t i = 0;i < columns;++i)
    {
        printf("%02X", i);
        if(i + 1 < columns)
        {
            printf(" ");
        }
        else
        {
            printf("\n");
        }
    }

    for(uint16_t i = 0;i < s_mem_len; i += columns)
    {
        uint16_t row_addr = i;

        for(uint16_t j = 0;j < columns; ++j)
        {
            uint16_t col_addr = row_addr + j;
    
            if(col_addr == s_mem_len)
            {
                break;
            }
            assert(col_addr <  s_mem_len);
    
            printf("%02X", s_mem[col_addr]);
            if(j + 1 < columns)
            {
                printf(" ");
            }
            else
            {
                printf("\n");
            }
        }
    }
}
#endif //NDEBUG
    
void mem_init(uint8_t * const mem, uint16_t const mem_len)
{
    s_mem = mem;
    s_mem_len = mem_len;

    g_mem_heap_len = s_mem_len - g_mem_addr_first;
}
