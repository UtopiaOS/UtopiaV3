#include <covenant/tq.h>

Status c_tq_pop_tail(TailQueue* self) {
    if (self->size == 0)
        return StatusErr;

    // Ensured tail is not nil
    // Since we have the "size" we can just iterate over size - 1, to ensure 
    // we have the previous one to the tail
    
    TailQueueNode* node = nil;
    TailQueueNode* prev = nil;

    // If the size is 1, we are again going to empty the queue
    if (self->size == 1) {
        node = self->tail;
        self->tail = self->head = nil;
        self->size = 0;
        if (self->destructor)
            self->destructor(node->data);
        c_std_free(node);
        return StatusOk;
    }

    // Find the previous element to the tail
    prev = self->head;
    for (Size i = 1; i < self->size - 1; i++) {
        prev = prev->next;
    }

    // Prev will be the next tail, and tail will be freed
    node = self->tail;
    self->tail = prev;
    self->tail->next = nil;
    self->size--;
    if (self->destructor)
        self->destructor(node->data);
    c_std_free(node);
    return StatusOk;
}