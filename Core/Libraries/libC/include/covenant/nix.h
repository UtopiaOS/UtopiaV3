
// TODO: Is this even a thing that is necessary?
// should we really wrap syscalls?


#define __NEED_usize
#define __NEED_i32
#define __NEED_size
#define __NEED_UChar
#define __NEED_ctype_fd
#define __NEED_USize
#define __NEED_Int32
#define __NEED_Size
#define __NEED_uchar
#define __NEED_FileDescriptor
#define __NEED_UniversalType
#define __NEED_Void
#include <covenant/bits/everytype.h>
#include <covenant/fn.h>
#include <covenant/status.h>

#define C_NIX_OREAD 0
#define C_NIX_OWRITE 1
#define C_NIX_ORDWR 2

#define C_NIX_OCREATE 0x40
#define C_NIX_OAPPEND 0x400

Void c_nix_munmap(UniversalType, USize);
Size c_nix_allrw(IOFunction, FileDescriptor, UniversalType, USize);
Status c_nix_fdset(FileDescriptor, Int32);