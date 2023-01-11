#include "internal/vec_impl.h"
#include <covenant/std.h>

Status vec_move_left(Vector* self, USize idx) {
    UniversalType offset = vec_get_offset_of(self, idx);

    USize elements_to_the_right_bytes = (self->size - idx - 1) * self->obj_size;

    c_mem_cpy(offset, elements_to_the_right_bytes, offset + self->obj_size);

    return StatusOk;
}