
// TODO: Is this even a thing that is necessary?
// should we really wrap syscalls?


#define __NEED_usize
#define __NEED_i32
#define __NEED_size
#define __NEED_uchar
#define __NEED_ctype_fd
#define __NEED_ctype_status
#include <covenant/bits/everytype.h>
#include <covenant/fn.h>
#include <covenant/status.h>

#define C_NIX_OREAD 0
#define C_NIX_OWRITE 1
#define C_NIX_ORDWR 2

#define C_NIX_OCREATE 0x40
#define C_NIX_OAPPEND 0x400

void c_nix_munmap(void*, usize);
size c_nix_allrw(ctype_iofn, ctype_fd, void*, usize);
ctype_status c_nix_fdset(ctype_fd, i32 flags);