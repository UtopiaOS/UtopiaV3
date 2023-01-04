#include <covenant/std.h>

// TODO: We should look for a way to merge get from file and get from ioq

Size
c_file_feed(File *fp)
{
    Size r;

    if (fp->data.capacity)
        return fp->data.capacity;

    if ((r = (fp->read_fn)(fp->fd, fp->data.members, fp->data.size)) <= 0)
        return r;
    
    fp->data.capacity = r;
    fp->data.size -= r;
    if (fp->data.size) {
        c_mem_cpy(fp->data.members + fp->data.size, r, fp->data.members);
    }

    return r;

}