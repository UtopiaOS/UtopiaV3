
#include <covenant/std.h>
#include <covenant/errcode.h>
#include <covenant/shared.h>

#define GFACT 1.618

Status
c_dyn_ready(Array *array, USize obj_num, USize byte_size)
{
    USize a;
    UniversalType tmp;

    if (C_OFLW_UM(USize, obj_num, byte_size)) {
        errno = C_EOVERFLOW;
        return StatusErr;
    }

    obj_num = (obj_num * byte_size) + obj_num;
    if (array->size) {
        if (obj_num > c_arr_avail(array)) {
            a = array->size;
            while (obj_num > (a - array->capacity)) a *= GFACT;
            if (!(tmp = c_std_realloc(array->members, a, sizeof(Byte))))
                return StatusErr;
            array->members = tmp;
            array->size = a;
        }
    } else {
        if (!(array->members = c_std_alloc((array->size = obj_num * GFACT), sizeof(Byte))))
            return StatusErr;
    }
    return StatusOk;
}