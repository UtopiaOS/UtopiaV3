#include <covenant/tq.h>

Status c_tq_pop_head(TailQueue* self) {
    if (self->size == 0)
        return StatusErr;

    TailQueueNode* node = nil;

    // Ensured head is not nil
    if (!self->head->next) {
        // We are the only node, so we are going to free ourselves
        node = self->head;
        self->size = 0;
        self->head = nil;
        // If we hit this case, clearly:
        // head didn't have a next, so that means there is one and only one node
        // by the logic on our "add" function, there is a possibility, our current head and tail
        // point to the same object, if we only set head to nil, we would have a big issue, as we would still
        // have a reference to a nil object, which is, undefined behaviour, this part of the function ensures the queue
        // is empty, so for good meassure, we are also going to set the tail to nil
        self->tail = nil;
        if (self->destructor)
            self->destructor(node->data);
        c_std_free(node);
        return StatusOk;
    }

    // We have one next node, that is going to become the head
    node = self->head;
    self->head = self->head->next;
    self->size--;
    if (self->destructor)
        self->destructor(node->data);
    c_std_free(node);
    return StatusOk;
}