#include <covenant/std.h>

UInt32
c_u32_unpack(char *s)
{
    UInt32 r;
    
	r = (UChar)s[3];
	r <<= 8;
	r += (UChar)s[2];
	r <<= 8;
	r += (UChar)s[1];
	r <<= 8;
	r += (UChar)s[0];
	return r;
}