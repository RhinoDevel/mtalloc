
// RhinoDevel, MT, 2018nov19

#ifndef MT_ALLOC
#define MT_ALLOC

#include <stdint.h>

void alloc_free(uint32_t const block_addr);

uint32_t alloc_alloc(uint32_t const wanted_len);

void alloc_init(uint8_t * const mem, uint32_t const mem_len);

#endif //MT_ALLOC
