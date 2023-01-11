#include <covenant/vec.h>
#include "internal/vec_impl.h"

Status vec_grow(Vector* self) {
    if (!self)
        return StatusErr;

    return vec_realloc(self, self->capacity * GROW_FACTOR);
} 