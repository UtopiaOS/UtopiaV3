# libCovenant

Utopia's libSystem equivalent.

libCovenant can be seen as an equivalent to macOS libSystem, as it includes inside it multiple libraries, like math, c, posix (the posix compat layer) and jemalloc.

## Parts

- libC: Utopia's default non-standard C library
- libPosix: Utopia's POSIX compatibility layer
- libJemalloc: Utopia's Jemalloc forked source
- libKernel: Utopia's syscall interface
- libMath: Utopia's math library
- libGeneric: Utopia's generic data structures (like hashmaps, dictionaries, tupples)
- libPthreads: Utopia's Pthreads implementation
