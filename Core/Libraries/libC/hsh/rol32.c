#include <covenant/std.h>
#include <covenant/hsh.h>

u32
c_hsh_rol32(u32 n, i32 k)
{
    u32 mask;
    mask = C_CHARBIT * sizeof(n) - 1;
    k &= mask;
	return ((n << k) | (n >> (-k & mask)));
}