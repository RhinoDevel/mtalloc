
// RhinoDevel, MT, 2018nov17

#include <stdio.h>
#include <stdlib.h>

#include "mtalloc/alloc.h"
#include "mtalloc/node.h"
#include "mtalloc/mem.h"

extern uint16_t g_mem_heap_len;

int main()
{
    static size_t const mem_len = 80;
    uint8_t * const mem = malloc(mem_len * sizeof *mem);

    alloc_init(mem, mem_len);

    uint16_t all = alloc_alloc(g_mem_heap_len - 2 * NODE_LEN);

    mem_clear(all, g_mem_heap_len - 2 * NODE_LEN, 0xBC);

    // Mem.Print();
    // Console.WriteLine("***");

    alloc_free(all);
    all = 0;

    uint16_t a = alloc_alloc(10);

    mem_clear(a, 10, 170);

    uint16_t b = alloc_alloc(5);

    mem_clear(b, 5, 187);

    uint16_t c = alloc_alloc(7);

    mem_clear(c, 7, 204);

    // Mem.Print();
    // Console.WriteLine("-");

    alloc_free(c);
    c = 0;

    // Mem.Print();
    // Console.WriteLine("-");

    alloc_free(b);

    // Mem.Print();
    // Console.WriteLine("-");

    alloc_free(a);

    // Mem.Print();

    free(mem);
    return 0;
}
