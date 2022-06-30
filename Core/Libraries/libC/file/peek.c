#include <covenant/std.h>

void *
c_file_peek(ctype_file* fp) 
{
    return (fp->data.members + fp->data.size);
}