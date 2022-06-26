#include <covenant/std.h>

#define EOF (-1)

ctype_status
c_file_close(ctype_file *fp) {
    int ret;

    ret = (c_file_flush(fp) | c_kernel_close(fp->fd)) ? EOF : 0;
    // TODO: Free the file memory
    // TODO: Do some unitialization
    // TODO: Unlock and lock the file, research what a mutex is
    return ret;
}