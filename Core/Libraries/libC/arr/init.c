#include <covenant/std.h>

Void
c_arr_init(Array *array, char *s, Size size)
{
    array->size = size ? size - 1 : 0;
    array->capacity = 0;
    array->members = (uchar *)s;
}