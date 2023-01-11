#include <covenant/vec.h>
#include <covenant/std.h>

Status c_vec_init(Vector* self, USize element_count, USize element_size) {
    if (!self)
        return StatusErr;
    self->capacity = element_count;
    self->size = 0;
    self->data = c_std_malloc(element_count * element_size);
    if (!self->data)
        return StatusErr;
    self->obj_size = element_size;
    return StatusOk;
}