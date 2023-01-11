#include <covenant/std.h>
#include <covenant/vec.h>
#include "internal/vec_impl.h"

Status c_vec_insert(Vector* self, USize idx, UniversalType element) {
    if (!self)
        return StatusErr;
    
    if (!element)
        return StatusErr;

    if (idx > self->size)
        return StatusErr;
    
    if (self->size == self->capacity)
        if (vec_grow(self) != StatusOk)
            return StatusErr;
    
    if (vec_move_right(self, idx) != StatusOk)
        return StatusErr;
    
    UniversalType offset = vec_get_offset_of(self, idx);
    c_mem_cpy(offset, self->obj_size, element);
    ++self->size;
    return StatusOk;
}