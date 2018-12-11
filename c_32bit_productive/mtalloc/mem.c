
// RhinoDevel, MT, 2018nov18

#include <stdint.h>
#include <assert.h>

#ifndef NDEBUG
#include <stdio.h>
#endif //NDEBUG

#include "mem.h"
#include "allocconf.h"

void mem_clear(void * const addr, MT_USIGN const len, uint8_t const val)
{
    uint8_t * const p = (uint8_t*)addr;
    
    for(MT_USIGN i = 0;i < len; ++i)
    {
        p[i] = val;
    }
}

#ifndef NDEBUG
void mem_print(void const * const mem, MT_USIGN const mem_len)
{   
    static char const * const f = sizeof(MT_USIGN) > 32 ? "%02lX" : "%02X";
    static MT_USIGN const columns = 16;
    uint8_t const * p = (uint8_t const *)mem;
    uint8_t const * const lim = p + mem_len;
    
    assert(columns < 256);

    for(MT_USIGN i = 0;i < columns;++i)
    {
        printf(f, i);
        if(i + 1 < columns)
        {
            printf(" ");
        }
        else
        {
            printf("\n");
        }
    }

    while(p < lim)
    {
        uint8_t const * row_lim = p + columns;
        
        if(row_lim > lim)
        {
            row_lim = lim;
        }
        
        while(p < row_lim)
        {
            printf(f, *p);
            
            if(p + 1 < row_lim)
            {
                printf(" ");
            }
            else
            {
                printf("\n");
            }
            
            ++p;
        }
    }
}
#endif //NDEBUG
