
// RhinoDevel, MT, 2018nov18

#include <stdint.h>

#ifndef NDEBUG
#include <assert.h>
#include <stdio.h>
#endif //NDEBUG

#include "mem.h"

uint32_t const g_mem_addr_first = 8; // Must not be 0?! Stupid..

static uint8_t * s_mem = 0;
static uint32_t s_mem_len = 0;

uint32_t g_mem_heap_len = 0;

static void fill(uint16_t val, uint8_t * const low, uint8_t * const high)
{
    *low = (uint8_t)(val & 0xFF);
    *high = (uint8_t)(val >> 8);
}

void mem_store_word(uint32_t const addr, uint16_t const val)
{
    fill(val, s_mem + addr, s_mem + addr + 1);
}

uint16_t mem_load_word(uint32_t const addr)
{
    return (uint16_t)(s_mem[addr] | (s_mem[addr + 1] << 8));
}

void mem_store_dword(uint32_t const addr, uint32_t const val)
{
    mem_store_word(addr, (uint16_t)(val & 0xFFFF));
    mem_store_word(addr + 2, (uint16_t)(val >> 16));
}

uint32_t mem_load_dword(uint32_t const addr)
{
    return (uint32_t)(mem_load_word(addr) | (mem_load_word(addr + 2) << 16));
}

void mem_store_byte(uint32_t const addr, uint8_t const val)
{
    s_mem[addr] = val;
}

uint8_t mem_load_byte(uint32_t const addr)
{
    return s_mem[addr];
}

void mem_clear(uint32_t const addr, uint32_t const len, uint8_t const val)
{
    uint32_t const lim = addr + len;

    for(uint32_t cur = addr;cur < lim; ++cur)
    {
        mem_store_byte(cur, val);
    }
}

#ifndef NDEBUG
void mem_print()
{   
    static uint32_t const columns = 16;
    
    assert(columns < 256);

    for(uint32_t i = 0;i < columns;++i)
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

    for(uint32_t i = 0;i < s_mem_len; i += columns)
    {
        uint32_t row_addr = i;

        for(uint32_t j = 0;j < columns; ++j)
        {
            uint32_t col_addr = row_addr + j;
    
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
    
void mem_init(uint8_t * const mem, uint32_t const mem_len)
{
    s_mem = mem;
    s_mem_len = mem_len;

    g_mem_heap_len = s_mem_len - g_mem_addr_first;
}
