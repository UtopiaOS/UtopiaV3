#include <covenant/vec.h>
#include "internal/vec_impl.h"

Status c_vec_push_back(Vector* self, UniversalType element) {
    if (!self)
        return StatusErr;
    if (!element)
        return StatusErr;
    
    if (self->capacity == self->size) {
        if (vec_grow(self) != StatusOk)
            return StatusErr;
    }

    if (vec_assign(self, self->size, element) != StatusOk)
        return StatusErr;

    ++self->size;
    return StatusOk;
}