

#include <covenant/std.h>

ctype_status
c_dyn_cat(ctype_arr *array, void *mem_area, usize obj_num, usize byte_size)
{
    if (!obj_num)
        return 0;
    
    if (c_dyn_ready(array, obj_num, byte_size) < 0)
        return -1;
    
    obj_num *= byte_size;
    c_mem_cpy(array->members + array->length, obj_num, mem_area);
    array->length += obj_num;
    array->members[array->length] = 0;
    return 0;
}