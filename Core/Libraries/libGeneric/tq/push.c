#include <covenant/tq.h>

Status c_tq_push(TailQueue* self, UniversalType element) {
    if (!element)
        return StatusErr;

    TailQueueNode* node = c_std_malloc(sizeof(TailQueueNode));
    if (!node)
        return StatusErr;

    Status status = StatusOk;
    if (!self->head) {
        status = c_tq_node_init(element, node);
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

    TailQueueNode* current_head = self->head;
    status = c_tq_node_init(element, node);
    if (status != StatusOk) {
        c_std_free(node);
        return status;
    }
    node->next = current_head;
    self->head = node;
    self->size++;
    return StatusOk;
}