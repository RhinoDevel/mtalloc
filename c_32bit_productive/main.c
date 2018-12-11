
// RhinoDevel, MT, 2018nov17

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "mtalloc/alloc.h"
#include "mtalloc/node.h"
#include "mtalloc/mem.h"

int main()
{   
    static size_t const mem_len = 200;
    uint8_t * const mem = malloc(mem_len * sizeof *mem);
    
    assert(sizeof *mem == 1);

    printf("Node size: %d\n", (int)sizeof(struct node));
    
    mem_clear(mem, mem_len, MT_ALLOC_DEB_CLR_1);
    mem_print(mem, mem_len);
    
    alloc_init(mem, mem_len);
    
    mem_print(mem, mem_len);

    void* all = alloc_alloc(mem_len - 2 * sizeof (struct node));

    mem_print(mem, mem_len);
    
    mem_clear(all, mem_len - 2 * sizeof (struct node), MT_ALLOC_DEB_CLR_2);

    mem_print(mem, mem_len);
    printf("***\n");

    alloc_free(all);
    all = 0;

    mem_print(mem, mem_len);
    
    void* a = alloc_alloc(10);

    mem_print(mem, mem_len);
    mem_clear(a, 10, 170);
    mem_print(mem, mem_len);

    void* b = alloc_alloc(5);

    mem_clear(b, 5, 187);

    void* c = alloc_alloc(7);

    mem_clear(c, 7, 204);

    mem_print(mem, mem_len);
    printf("-\n");

    alloc_free(c);
    c = 0;

    mem_print(mem, mem_len);
    printf("-\n");

    alloc_free(b);

    mem_print(mem, mem_len);
    printf("-\n");// Console.WriteLine("-");

    alloc_free(a);

    mem_print(mem, mem_len);

    free(mem);
    
    return 0;
}
