#include <covenant/std.h>

// TODO: We should look for a way to merge get from file and get from ioq


size
c_file_get(ctype_file *fp, char *buf, usize n)
{
    usize len, min;
    size r;

    len = n;
    while (len)
    {
        if ((r = c_file_feed(fp))  < 0)
            return -1;
        if (!r)
            break;

        min = C_MIN((usize)r, len);
        c_mem_cpy(buf, min, fp->data.members + fp->data.size);
        fp->data.length -= min;
        fp->data.size += min;
        len -= min;
        buf += min;
    }

    return len ? n - len : n;
}