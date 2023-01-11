#include "internal/vec_impl.h"

UniversalType vec_get_offset_of(Vector* self, USize idx) {
    return self->data + (idx * self->obj_size);
}