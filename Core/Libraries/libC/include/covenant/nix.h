
// TODO: Is this even a thing that is necessary?
// should we really wrap syscalls?

#define __NEED_usize
#define __NEED_i32
#define __NEED_size
#define __NEED_uchar
#define __NEED_ctype_fd
#include <covenant/bits/everytype.h>
#include <covenant/fn.h>

void c_nix_munmap(void*, usize);
size c_nix_allrw(ctype_iofn, ctype_fd, void*, usize);