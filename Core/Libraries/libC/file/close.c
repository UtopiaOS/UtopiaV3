#include <covenant/std.h>

#define EOF (-1)

Status
c_file_close(File *fp) {
    int ret;

    ret = (c_file_flush(fp) | c_kernel_close(fp->fd)) ? EOF : 0;
    // TODO: Free the file memory
    // TODO: Do some unitialization
    // TODO: Unlock and lock the file, research what a mutex is
    return ret;
}