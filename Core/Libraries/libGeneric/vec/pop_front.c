#include <covenant/vec.h>

Status c_vec_pop_front(Vector* self) {
    return c_vec_erase(self, 0);
}