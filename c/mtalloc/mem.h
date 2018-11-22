
// RhinoDevel, MT, 2018nov18

#ifndef MT_MEM
#define MT_MEM

#include <stdint.h>

void mem_store_word(uint16_t const addr, uint16_t const val);
uint16_t mem_load_word(uint16_t const addr);

void mem_store_byte(uint16_t const addr, uint8_t const val);
uint8_t mem_load_byte(uint16_t const addr);

void mem_clear(uint16_t const addr, uint16_t const len, uint8_t const val);

/** Initialize singleton.
 */
void mem_init(uint8_t * const mem, uint16_t const mem_len);

#endif //MT_MEM
