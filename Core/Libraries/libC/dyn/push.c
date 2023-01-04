

#include <covenant/std.h>

Status
c_dyn_push(ctype_arr *array, void *mem_area, usize obj_num, usize byte_size)
{
    if (!obj_num)
        return StatusOk;
    
    if (c_dyn_ready(array, obj_num, byte_size) < 0)
        return StatusErr;
    
    obj_num *= byte_size;
    c_mem_cpy(array->members + array->length, obj_num, mem_area);
    array->length += obj_num;
    array->members[array->length] = 0;
    return StatusOk;
}