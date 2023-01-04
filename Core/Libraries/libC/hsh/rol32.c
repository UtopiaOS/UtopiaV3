#include <covenant/std.h>
#include <covenant/hsh.h>

UInt32
c_hsh_rol32(UInt32 n, Int32 k)
{
    UInt32 mask;
    mask = C_CHARBIT * sizeof(n) - 1;
    k &= mask;
	return ((n << k) | (n >> (-k & mask)));
}