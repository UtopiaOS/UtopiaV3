#include <covenant/std.h>
#include "internal/vec_impl.h"

Status c_vec_assign(Vector* self, USize idx, UniversalType element) {
    if (!self)
        return StatusErr;
    
    if (!element)
        return StatusErr;
    
    if (idx >= self->size)
        return StatusErr;

    vec_assign(self, idx, element);

    return StatusOk;
}