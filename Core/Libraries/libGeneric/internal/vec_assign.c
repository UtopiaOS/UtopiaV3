#include "internal/vec_impl.h"
#include <covenant/std.h>

Status vec_assign(Vector* self, USize idx, UniversalType element) {
    UniversalType offset = vec_get_offset_of(self, idx);
    if (!offset)
        return StatusErr;
    c_mem_cpy(offset, self->obj_size, element);
    return StatusOk;
}