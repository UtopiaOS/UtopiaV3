#include <covenant/vec.h>
#include <covenant/std.h>

UniversalType c_vec_back(Vector *self) {
    if (!self)
        return nil;

    return c_vec_get(self, self->size - 1);
}