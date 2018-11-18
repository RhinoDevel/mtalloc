
// RhinoDevel, MT, 2018nov17

#ifndef MT_NODEMEM
#define MT_NODEMEM

/** Node at given address must be unallocated!
 */
void nodemem_merge_unallocated_with_next_if_possible(
    uint16_t const unallocated_node_addr);

void nodemem_limit_free_nodes();

/** Occupy heap space with one node object that reserves the whole rest
 *  of heap space as one single unallocated node.
 */
void nodemem_init();

#endif //MT_NODEMEM
