

#include <covenant/std.h>
#include <covenant/shared.h>
#include <covenant/errcode.h>

Status
c_arr_trunc(Array *array, USize obj_num, USize obj_size)
{
    if (C_OFLW_UM(usize, obj_num, obj_size)) {
        errno = C_EOVERFLOW;
        return StatusErr;
    }

    if ((obj_num *= obj_size) > c_arr_bytes(array))
        return StatusOk;
    
    array->capacity = obj_num;
    if (array->members)
        array->members[array->capacity] = 0;
    
    return StatusErr;
}