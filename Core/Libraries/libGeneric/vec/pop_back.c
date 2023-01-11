#include <covenant/vec.h>

Status c_vec_pop_back(Vector* self) {
    if (!self)
        return StatusErr;
    
    --self->size;
    return StatusOk;
}