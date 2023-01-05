#include <covenant/tq.h>

Status c_tq_node_init(UniversalType element, TailQueueNode* node) {
    node->data = element;
    node->next = nil; // Maybe a function that allows this
    return StatusOk;
}