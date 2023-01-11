#include <covenant/std.h>
#include <covenant/bool.h> // TODO: Mode bool into STD

Bool
c_str_equal(char* first, USize size, char* second) {
    return (c_str_cmp(first, size, second) == 0);
}