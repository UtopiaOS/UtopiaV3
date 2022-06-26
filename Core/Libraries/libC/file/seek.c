#include <covenant/std.h>


// TODO: Is this implementation enough?
// Move on a file n bytes
void
c_file_seek(ctype_file *p, usize n)
{
    p->data.members += n;
    p->data.length -= n;
}

