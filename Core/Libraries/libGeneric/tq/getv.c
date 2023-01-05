#include <covenant/tq.h>

UniversalType c_tq_getv(TailQueue* self, UniversalType element) {
    if (self->size == 0)
        return nil;

    if (!element)
        return nil;

    if (!self->cmp)
        return nil;
    
    TailQueueNode* node = self->head;
    while (node) {
        if (self->cmp(node->data, element)) {
            return node->data;
        }
        node = node->next;
    }
    return nil;
}