#include <covenant/std.h>
#include <covenant/errcode.h>
#include <covenant/shared.h>

Status
c_arr_ready(Array *array, USize m, USize n) {
    if (C_OFLW_UM(USize, m, n)) {
        errno = C_EOVERFLOW;
        return StatusErr;    
    }
	if ((m = (m * n) + n) > c_arr_avail(array)) {
        errno = C_EOVERFLOW;
        return StatusErr;
    }
    return StatusOk;
}