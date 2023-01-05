#include <covenant/tq.h>

Status c_tq_add(TailQueue* self, UniversalType obj) {
    if (!obj)
        return StatusErr;
    

    TailQueueNode* node = c_std_malloc(sizeof(TailQueueNode));
    if (!node)
        return StatusErr;

    Status status = StatusOk;

    // We are empty, so this node will be both our head and tail
    if (!self->head) {
        status = c_tq_node_init(obj, node);
        if (status != StatusOk) {
            c_std_free(node);
            return status;
        }
        self->size++;
        self->head = node;
        self->tail = self->head;
        self->tail->next = nil;
        return StatusOk;
    }

    // We are going to be the new tail
    TailQueueNode *current_tail = self->tail;
    status = c_tq_node_init(obj, node); // New tail
    if (status != StatusOk) {
        c_std_free(node);
        return status;
    }
    current_tail->next = node;
    self->tail = node;
    self->size++;
    return StatusOk;
}