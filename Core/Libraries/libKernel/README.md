# libKernel

libKernel is Utopia's lowest userland layer, it is in charge of performing diverse syscall routines to the kernel, it also properly maps unique identifiers depending on their implementation.

Utopia supports Linux and Mach syscalls (the later not being fully implemented as of yet), libKernel takes a unique approach, similar to the way macOS does it, our Mach syscalls are represented in negative integers, while linux syscalls are represented in positive integers.