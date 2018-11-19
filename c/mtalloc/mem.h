
// RhinoDevel, MT, 2018nov18

#ifndef MT_MEM
#define MT_MEM

#include <stdint.h>

void mem_store_word(uint16_t const addr, uint16_t const val);
uint16_t mem_load_word(uint16_t const addr);

void mem_store_byte(uint16_t const addr, uint8_t const val);
uint8_t mem_load_byte(uint16_t const addr);

/** Initialize singleton.
 */
void mem_init(uint8_t * const heap, uint16_t const heap_len);

#endif //MT_MEM
