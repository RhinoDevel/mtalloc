
// RhinoDevel, MT, 2018nov18

#ifndef MT_NODE
#define MT_NODE

#include <stdint.h>

#define NODE_FREE_FLAG_WORD 0xFFFFFFFF

#define NODE_LEN 17
//
struct node
{
    uint32_t last_node_addr;
    uint32_t block_addr;
    uint32_t block_len;
    uint8_t is_allocated;
    uint32_t next_node_addr;
};

void node_fill(uint32_t const node_addr, struct node * const n);
void node_store(uint32_t node_addr, struct node const * const n);

#endif //MT_NODE
