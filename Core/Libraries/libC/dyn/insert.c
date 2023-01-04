

#include <covenant/std.h>

Status
c_dyn_insert(Array *array, USize pos, UniversalType v, USize m, USize n)
{
    USize len;
    Byte *target;

    if (c_dyn_ready(array, m, n) < 0)
        return StatusErr;
    len = c_arr_bytes(array);
    if (!(target = c_dyn_alloc(array, pos, n)))
        return StatusErr;

    if (pos)
        pos = (pos - 1) * n;
    m *= n;
    if (len > (pos + n))
    {
        c_mem_cpy(target + m, len - pos, target);
        array->capacity += m;
    }
    else
    {
        array->capacity += m - n;
    }
    c_mem_cpy(target, m, v);
    array->members[array->capacity] = 0;
    return StatusOk;
}
