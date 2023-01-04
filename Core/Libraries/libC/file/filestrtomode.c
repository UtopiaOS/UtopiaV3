#include "internal/_modes.h"

Int32
__covenant_filestrtomode(const char *s)
{
    Int32 mode;

    if (!s)
        return -1;
    
    mode = 0;

    switch(s[0]) {
        case 'r':
            mode |= C_NIX_OREAD;
            break;
        case 'w':
            mode |= C_NIX_OCREATE | C_NIX_OWRITE;
            break;
        case 'a':
            mode |= C_NIX_OAPPEND | C_NIX_OCREATE | C_NIX_OWRITE;
            break;
        default:
            return -1;
    }

    switch (s[1]) {
        case 'b':
            break;
        case '+':
            mode |= C_NIX_ORDWR;
            break;
        case '\0':
            return mode;
        default:
            return -1;
    }

    switch(s[2]) {
        case 'b':
            return (!s[3]) ? mode: -1;
        case '\0':
            return mode;
        default:
            return -1;
    }
}