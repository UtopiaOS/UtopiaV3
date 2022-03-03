

#include <covenant/kernel/basic.h>
#include <covenant/nix.h>
#include <covenant/shared.h>

void
c_nix_munmap(void *p, usize n)
{
    i32 sverr;

    sverr = errno;
    c_kernel_munmap(p, n);
    errno = sverr;
}