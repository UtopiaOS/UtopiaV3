#include "internal/_modes.h"
#include <covenant/std.h>
#include <covenant/kernel/basic.h>
#include "internal/_fdopen.h"
#include "internal/attributes.h"

ctype_file *
c_file_open(const char *filename, const char* mode)
{
    ctype_file *f;
    i32 flags;
    i32 fd;
    if (!c_str_chr("rwa", 3, *mode)) {
        return nil;
    }

    // Compute the flags to pass to open syscall
    flags = __covenant_filestrtomode(mode);
    
    fd = c_kernel_open3(UTOPIA_KILL_CONSTANT(char*, filename), flags, 0666);
    if (fd < 0 ) {
        return nil;
    }

    // TODO: Implement a case where the flags are O_CLOEXEC
    f = __covenant_open(fd, mode);
    if (f)
        return f;

    // We failed, tell the kernel about it
    c_kernel_close(fd);
    return nil;
}