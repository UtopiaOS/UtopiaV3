#include "internal/vec_impl.h"
#include <covenant/std.h>

Status vec_realloc(Vector* self, USize new_capacity) {
    UniversalType current_data = self->data;
    
    USize new_capacity_bytes = new_capacity * self->obj_size;

    self->data = c_std_malloc(new_capacity_bytes);
    if (!self->data)
        return StatusErr;
    
    c_mem_cpy(self->data, self->size * self->obj_size, current_data);

    self->capacity = new_capacity;
    c_std_free(current_data);
    return StatusOk;
}