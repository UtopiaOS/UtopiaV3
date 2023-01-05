#include <covenant/tq.h>

Status c_tq_insert(TailQueue* self, Size idx, UniversalType element) {
    if (idx == self->size)
        return c_tq_add(self, element);

    if (idx == 0)
        return c_tq_push(self, element);

    Status status = StatusOk;
        
    // TODO: Is this right?
    TailQueueNode* prev = c_tq_getnat(self, idx - 1);
    if (!prev)
        return StatusErr;

    TailQueueNode* insert_tgt = c_std_malloc(sizeof(TailQueueNode));
    if (!insert_tgt)
        return StatusErr;

    status = c_tq_node_init(element, insert_tgt);
    if (status != StatusOk)
        return status;
    
    TailQueueNode* displaced = prev->next;
    prev->next = insert_tgt;
    insert_tgt->next = displaced;

    self->size++;
    return StatusOk;
}