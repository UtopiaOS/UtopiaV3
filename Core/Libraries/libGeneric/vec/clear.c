#include <covenant/std.h>
#include "internal/vec_impl.h"

Status c_vec_clear(Vector* self) {
    return c_vec_resize(self, 0);
}