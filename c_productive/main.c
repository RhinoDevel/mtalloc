
// RhinoDevel, MT, 2018nov17

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "mtalloc/alloc.h"
#include "mtalloc/node.h"
#include "mtalloc/mem.h"

int main()
{  
    // Allocate memory via system for testing:
    
    static size_t const mem_len = 200;
    uint8_t * const mem = malloc(mem_len * sizeof *mem);
    
    assert(sizeof *mem == 1);
    assert(sizeof (void*) == sizeof (MT_USIGN));

#ifndef NDEBUG
    printf("Node size: %d\n", (int)sizeof (struct node));
#endif //NDEBUG
    
#ifndef NDEBUG
    mem_clear(mem, mem_len, MT_ALLOC_DEB_CLR_1);
    mem_print(mem, mem_len);
#endif //NDEBUG
    
    // Initialize memory heap manager:
    
    alloc_init(mem, mem_len);

#ifndef NDEBUG
    mem_print(mem, mem_len);
#endif //NDEBUG

    // Reserve all possible memory:
    
    void* all = alloc_alloc(mem_len - 2 * sizeof (struct node));

#ifndef NDEBUG
    mem_print(mem, mem_len);
#endif //NDEBUG
    
#ifndef NDEBUG
    mem_clear(all, mem_len - 2 * sizeof (struct node), MT_ALLOC_DEB_CLR_2);
    mem_print(mem, mem_len);
    printf("***\n");
#endif //NDEBUG

    alloc_free(all);
    all = 0;

#ifndef NDEBUG
    mem_print(mem, mem_len);
#endif //NDEBUG
    
    // Some more testing:
    
    void* a = alloc_alloc(10);

#ifndef NDEBUG
    mem_print(mem, mem_len);
#endif //NDEBUG
    
#ifndef NDEBUG
    mem_clear(a, 10, 170);
    mem_print(mem, mem_len);
#endif //NDEBUG

    void* b = alloc_alloc(5);

#ifndef NDEBUG
    mem_clear(b, 5, 187);
#endif //NDEBUG
    
    void* c = alloc_alloc(7);

#ifndef NDEBUG
    mem_clear(c, 7, 204);
    mem_print(mem, mem_len);
    printf("-\n");
#endif //NDEBUG
    
    alloc_free(c);
    c = 0;

#ifndef NDEBUG
    mem_print(mem, mem_len);
    printf("-\n");
#endif //NDEBUG

    alloc_free(b);

#ifndef NDEBUG
    mem_print(mem, mem_len);
    printf("-\n");
#endif //NDEBUG

    alloc_free(a);

#ifndef NDEBUG
    mem_print(mem, mem_len);
#endif //NDEBUG
    
    // Release allocated memory:
    
    free(mem);
    
    return 0;
}
