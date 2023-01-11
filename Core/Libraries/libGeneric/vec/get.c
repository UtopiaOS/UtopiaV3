#include <covenant/vec.h>
#include <covenant/std.h>
#include "internal/vec_impl.h"

UniversalType c_vector_get(Vector* self, USize idx) {
    if (!self)
        return nil;
    
    if (idx >= self->size)
        return nil;
    
    return vec_get_offset_of(self, idx);
}