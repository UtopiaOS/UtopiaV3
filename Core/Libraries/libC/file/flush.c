#include <covenant/std.h>
#include <covenant/nix.h>

ctype_status
c_file_flush(ctype_file *fp) {
    size ret;

    if ((ret = c_nix_allrw(fp->writefn, fp->fd, fp->data.members, fp->data.length)) < 0)
        return -1;

    c_arr_trunc(&fp->data, fp->data.length - ret, sizeof(uchar));
    return 0;
}