#include <covenant/tq.h>

Status c_tq_init(TailQueue* self, Size obj_size, CompareFunction cmp, DestructorFunction destructor) {
    if (!self)
        return StatusErr;

    self->head = self->tail = nil;
    self->size = 0;
    self->obj_size = obj_size;
    self->cmp = cmp;
    self->destructor = destructor;
    return StatusOk;
}

