#include <covenant/tq.h>

UniversalType c_tq_getvat(TailQueue* self, Size idx) {
    if (self->size == 0)
        return nil;
    
    if (idx > self->size)
        return nil;

    TailQueueNode* node = self->head;
    for (Size i = 1; i <= idx && node; i++) {
        node = node->next;
    }

    return node->data;
}