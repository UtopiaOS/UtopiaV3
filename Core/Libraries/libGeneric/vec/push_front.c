#include <covenant/std.h>
#include <covenant/vec.h>

Status c_vec_push_front(Vector* self, UniversalType element) {
    return c_vector_insert(self, 0, element);
}