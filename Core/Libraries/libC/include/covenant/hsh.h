

#define __NEED_i32
#define __NEED_u32
#define __NEED_usize
#include <covenant/bits/everytype.h>
#include <covenant/dat.h>

typedef void (*hfunc)(HST *, char*);

void c_hsh_murmur3(const void*, const Int32, const UInt32, void*);
UInt32 c_hsh_rol32(UInt32, Int32);
USize c_hsh_octets(HST*);
void c_hsh_update(hfunc, Int32, HST*, char*, USize);
extern HMD *c_hsh_murmur32;