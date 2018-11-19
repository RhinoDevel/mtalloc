
// RhinoDevel, MT, 2018nov19

#ifndef MT_ALLOC
#define MT_ALLOC

#include <stdint.h>

void alloc_free(uint16_t const block_addr);

uint16_t alloc_alloc(uint16_t const wanted_len);

void alloc_init(uint8_t * const heap, uint16_t const heap_len);

#endif //MT_ALLOC
