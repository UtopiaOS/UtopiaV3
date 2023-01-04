#include <covenant/std.h>
#include <covenant/shared.h>

Status
c_arr_push(Array *array, UniversalType data, USize m, USize n) {
    if (m == 0)
        return StatusOk;

    if (c_arr_ready(array, m, n) < 0)
        return StatusErr; 

    m *= n;

    c_mem_cpy(array->members + array->capacity, m, data);
    array->capacity += m;
    c_mem_set(array->members + array->capacity, n, 0);
    return StatusOk;
}