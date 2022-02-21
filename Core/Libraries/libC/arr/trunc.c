

#include <covenant/std.h>
#include <covenant/shared.h>
#include <covenant/errcode.h>

ctype_status
c_arr_trunc(ctype_arr *array, usize obj_num, usize obj_size)
{
    if (C_OFLW_UM(usize, obj_num, obj_size)) {
        errno = C_EOVERFLOW;
        return -1;
    }

    if ((obj_num *= obj_size) > c_arr_bytes(array))
        return 0;
    
    array->length = obj_num;
    if (array->members)
        array->members[array->length] = 0;
    
    return 0;
}