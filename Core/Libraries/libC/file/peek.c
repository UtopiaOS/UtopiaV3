#include <covenant/std.h>

UniversalType
c_file_peek(File* fp) 
{
    return (fp->data.members + fp->data.size);
}