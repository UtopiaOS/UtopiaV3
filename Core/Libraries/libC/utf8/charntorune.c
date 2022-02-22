
#include <covenant/std.h>

static uchar tab[] = {
    0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 0, 0,
};

extern uchar *__utf8_mtab;

i32
c_utf8_charntorune(ctype_rune *p, char *s, usize len)
{
    ctype_rune rune;
    usize i, n;
    uchar c;

    c = *(uchar *)s;
    if (c <= 0x80) {
        *p = c;
        return 1;
    }

    if (!(n = C_MIN(len, tab[c & 0x3F]))) {
        n = 1;
        goto bad;
    }

    rune = c & __utf8_mtab[n - 1];
    for (i = 1; i < n; i++) {
        if (((uchar)s[i] ^ 0x80) & 0xC0) {
            n = i;
            goto bad;
        }
        rune = ( rune << 6 ) | ((uchar)s[i] & 0x3F); 
    }

    if (c_utf8_checkrune(rune) < 0)
        goto bad;
    
    goto done;

bad:
    rune = C_RUNEERROR;
done:
    *p = rune;
    return n;
}