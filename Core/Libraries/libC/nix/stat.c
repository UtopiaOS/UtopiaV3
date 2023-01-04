#include <covenant/kernel/basic.h>
#include <covenant/nix.h>
#include <covenant/shared.h>

// Just a "wrapper" this function just calls the kernel stat, which just syscalls
Status c_nix_stat(char* filename, Stat *stat) {
    return c_kernel_stat(filename, stat);
}