
// RhinoDevel, MT, 2018nov18

#ifndef MT_NODE
#define MT_NODE

#include <stdint.h>

#define NODE_FREE_FLAG_WORD 0xFFFF

#define NODE_LEN 9
//
struct node
{
    uint16_t last_node_addr;
    uint16_t block_addr;
    uint16_t block_len;
    uint8_t is_allocated;
    uint16_t next_node_addr;
};

void node_fill(uint16_t const node_addr, struct node * const n);
void node_store(uint16_t node_addr, struct node const * const n);

#endif //MT_NODE
