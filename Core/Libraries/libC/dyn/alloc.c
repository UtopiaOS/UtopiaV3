

#include <covenant/std.h>

UniversalType
c_dyn_alloc(Array *array, USize obj_num, USize obj_size)
{
    USize t;

    if (c_dyn_ready(array, obj_num, obj_size) < 0)
        return nil;

    obj_num *= obj_size;
    if ((t = obj_num + obj_size) > c_arr_bytes(array))
        array->capacity = t;
    
    return array->members + obj_num;
}