#include <covenant/std.h>

u32
c_u32_unpack(char *s)
{
    u32 r;
    
	r = (uchar)s[3];
	r <<= 8;
	r += (uchar)s[2];
	r <<= 8;
	r += (uchar)s[1];
	r <<= 8;
	r += (uchar)s[0];
	return r;
}