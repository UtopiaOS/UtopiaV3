#include <covenant/vec.h>
#include "internal/vec_impl.h"

Status c_vec_erase(Vector* self, USize idx) {
    if (!self)
        return StatusErr;
    
    if (idx >= self->size)
        return StatusErr;

    vec_move_left(self, idx);

    // TODO: Shrink
    return StatusOk;
}