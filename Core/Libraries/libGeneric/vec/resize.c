#include <covenant/vec.h>
#include "internal/vec_impl.h"

Status c_vec_resize(Vector* self, USize new_size) {
    if (!self)
        return StatusErr;

    if (new_size <= self->capacity * SHRINK_THRESHOLD) {
        self->size = new_size;
        return vec_realloc(self, new_size);
    }    

    if (new_size > self->capacity)
        return vec_grow(self);

    self->size = new_size;
    return StatusOk;
}