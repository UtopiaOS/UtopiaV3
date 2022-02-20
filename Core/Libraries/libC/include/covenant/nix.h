
// TODO: Is this even a thing that is necessary?
// should we really wrap syscalls?

#define __NEED_usize
#define __NEED_i32
#include <covenant/bits/everytype.h>

void c_nix_munmap(void*, usize);