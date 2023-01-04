#include <covenant/std.h>
#include <covenant/nix.h>

Status
c_file_flush(File *fp) {
    Size ret;

    if ((ret = c_nix_allrw(fp->write_fn, fp->fd, fp->data.members, fp->data.capacity)) < 0)
        return -1;

    c_arr_trunc(&fp->data, fp->data.capacity - ret, sizeof(Byte));
    return 0;
}