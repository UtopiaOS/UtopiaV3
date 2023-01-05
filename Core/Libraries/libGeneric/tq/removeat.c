#include <covenant/tq.h>

Status c_tq_removeat(TailQueue* self, Size idx) {
    if (self->size == 0)
        return StatusErr;

    if (idx > self->size)
        return StatusErr;

    // Remove the first element, aka the head
    if (idx == 0) {
        return c_tq_pop_head(self);   
    }

    if (idx == self->size - 1) {
        return c_tq_pop_tail(self);
    }


    /* This removal is ensured to not be: The tail or head and to be in range*/
    TailQueueNode* current = self->head;
    TailQueueNode* tmp = nil;

    for (Size i = 1; i < idx && current; i++) {
        current = current->next;
    }

    tmp = current->next;
    current->next = tmp->next;
    if (self->destructor)
        self->destructor(tmp->data);
    c_std_free(tmp);

    self->size--;
    return StatusOk;
}