#include <covenant/tq.h>

TailQueueNode* c_tq_getn(TailQueue* self, UniversalType element) {
    if (self->size == 0)
        return nil;
    
    if (!element)
        return nil;

    if (!self->cmp)
        return nil;
    
    TailQueueNode* node = self->head;
    while (node) {
        if (self->cmp(node->data, element)) {
            return node;
        }
        node = node->next;
    }
    return nil;
}