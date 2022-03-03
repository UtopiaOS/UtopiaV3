
#include <covenant/std.h>
#include <covenant/errcode.h>
#include <covenant/shared.h>

#define GFACT 1.618

ctype_status
c_dyn_ready(ctype_arr *array, usize obj_num, usize byte_size)
{
    usize a;
    void *tmp;

    if (C_OFLW_UM(usize, obj_num, byte_size)) {
        errno = C_EOVERFLOW;
        return -1;
    }

    obj_num = (obj_num * byte_size) + obj_num;
    if (array->size) {
        if (obj_num > c_arr_avail(array)) {
            a = array->size;
            while (obj_num > (a - array->length)) a *= GFACT;
            if (!(tmp = c_std_realloc(array->members, a, sizeof(uchar))))
                return -1;
            array->members = tmp;
            array->size = a;
        }
    } else {
        if (!(array->members = c_std_alloc((array->size = obj_num * GFACT), sizeof(uchar))))
            return -1;
    }
    return 0;
}