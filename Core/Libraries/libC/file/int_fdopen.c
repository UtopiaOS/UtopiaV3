#include <covenant/std.h>
#include <covenant/sys/syscall.h>
#include "syscall.h"
#include <covenant/nix.h>
#include "internal/attributes.h"
#include <covenant/kernel/basic.h>

UTOPIA_ALWAYS_INLINE void __covenant_file_init(File* f, FileDescriptor fd, char *buf, USize n) {
    c_arr_init(&f->data, buf, n);
    f->write_fn = c_kernel_write;
    f->read_fn = c_kernel_read;
    f->fd = fd;
}

File *
__covenant_open(FileDescriptor fd, const char *mode)
{
    File *f;
    Status status;

    if (!c_str_chr("rwa", 3, *mode)) {
        return nil;
    }

    // TODO: We should create a file struct that also contains the flags?

    /* Set append */
    if (*mode == 'a') {
        // TODO: Write a nix wrapper around this
        Int32 flags = __syscall(SYS_fcntl, fd, C_KERNEL_FGETFL);
        if (!(flags & C_NIX_OAPPEND)) {
            status = c_nix_fdset(fd, flags | C_NIX_OAPPEND);
            if (status < 0)
                return nil;
        }
    }

    // God knows why we allocate like this, in fact, I would know why
    // but I didn't document it for other buffers, so let's just asume I know 
    // what this means LOL
    if (!(f = c_std_malloc(C_BIOSIZ + sizeof(*f))))
        return nil;

    // We are almost done initialize
    __covenant_file_init(f, fd, (UniversalType)((uchar*)f + sizeof(*f)), C_BIOSIZ);
    return f;
}