

#include <covenant/std.h>

Status
c_dyn_push(Array *array, UniversalType mem_area, USize obj_num, USize byte_size)
{
    if (!obj_num)
        return StatusOk;
    
    if (c_dyn_ready(array, obj_num, byte_size) < 0)
        return StatusErr;
    
    obj_num *= byte_size;
    c_mem_cpy(array->members + array->capacity, obj_num, mem_area);
    array->capacity += obj_num;
    array->members[array->capacity] = 0;
    return StatusOk;
}