

#include <covenant/std.h>

void *
c_dyn_alloc(ctype_arr *array, usize obj_num, usize obj_size)
{
    usize t;

    if (c_dyn_ready(array, obj_num, obj_size) < 0)
        return nil;

    obj_num *= obj_size;
    if ((t = obj_num + obj_size) > c_arr_bytes(array))
        array->length = t;
    
    return array->members + obj_num;
}