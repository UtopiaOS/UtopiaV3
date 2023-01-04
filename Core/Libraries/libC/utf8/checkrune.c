
#include <covenant/std.h>

Status
c_utf8_checkrune(Rune rune)
{
    if (rune > C_RUNEMAX ||
        (rune & 0xFFFE) == 0xFFFE ||
        (rune >= 0xD800 && rune <= 0xDFFF) ||
        (rune >= 0xFDD0 && rune <= 0xFDEF) ||
        (rune > 0x10FFFF))
        return -1;
    return 0;
}