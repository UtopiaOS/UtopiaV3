#include <covenant/vec.h>

UniversalType c_vec_front(Vector* self) {
    return c_vec_get(self, 0);
}