

#define __NEED_i32
#define __NEED_u32
#define __NEED_usize
#include <covenant/bits/everytype.h>

void c_hsh_murmur3(const void*, const i32, const u32, void*);
u32 c_hsh_rol32(u32, i32);
usize c_hsh_octets(ctype_hst*);
extern ctype_hmd *c_hsh_murmur32;