#include <covenant/std.h>

void start(void) __asm__("start");

__attribute__((force_align_arg_pointer))
void start(void) {
    
    c_std_exit(0);
}