

#include <covenant/std.h>

USize
c_arr_avail(Array *array)
{
    return (array->size - array->capacity);
}