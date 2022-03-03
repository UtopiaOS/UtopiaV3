

#include <covenant/std.h>

void start(void) __asm__("start");


int main() {
    
}

void start(void) {
    c_ioq_fmt(ioq2, "Hello world!\n");
    c_std_exit(0);
}