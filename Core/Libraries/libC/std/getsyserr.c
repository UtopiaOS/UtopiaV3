
#include <covenant/std.h>

char *(*__syserr)(void);
static char errbuf[C_ERRSIZ];

char *
c_std_getsyserr(void)
{
    return __syserr ? __syserr() : errbuf;
}