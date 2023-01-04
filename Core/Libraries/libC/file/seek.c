#include <covenant/std.h>


// TODO: Is this implementation enough?
// Move on a file n bytes
void
c_file_seek(File *p, USize n)
{
    p->data.members += n;
    p->data.capacity -= n; // Why would the capacity change as we seek?
}