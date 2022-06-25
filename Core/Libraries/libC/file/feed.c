#include <covenant/std.h>

// TODO: We should look for a way to merge get from file and get from ioq

size
c_file_feed(ctype_file *fp)
{
    size r;

    if (fp->data.length)
        return fp->data.length;

    if ((r = (fp->readfn)(fp->fd, fp->data.members, fp->data.size)) <= 0)
        return r;
    
    fp->data.length = r;
    fp->data.size -= r;
    if (fp->data.size) {
        c_mem_cpy(fp->data.members + fp->data.size, r, fp->data.members);
    }

    return r;

}