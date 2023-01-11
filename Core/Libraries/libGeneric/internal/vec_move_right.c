#include "internal/vec_impl.h"
#include <covenant/std.h>

Status vec_move_right(Vector* self, USize idx) {
    UniversalType offset = vec_get_offset_of(self, idx);

    USize displacement_amount = (self->size - idx) * self->obj_size;

    c_mem_cpy(offset + self->obj_size, displacement_amount, offset);
    return StatusOk;
}