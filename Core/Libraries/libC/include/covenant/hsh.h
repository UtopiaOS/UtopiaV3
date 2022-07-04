

#define __NEED_i32
#define __NEED_u32
#define __NEED_usize
#include <covenant/bits/everytype.h>
#include <covenant/dat.h>

typedef void (*hfunc)(ctype_hst *, char*);

void c_hsh_murmur3(const void*, const i32, const u32, void*);
u32 c_hsh_rol32(u32, i32);
usize c_hsh_octets(ctype_hst*);
void c_hsh_update(hfunc, i32, ctype_hst*, char*, usize);
extern ctype_hmd *c_hsh_murmur32;