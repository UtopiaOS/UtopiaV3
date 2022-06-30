#include <covenant/std.h>

void mdlk_actual_start(int argc, const char* argv[], const char* envp[])
{
    ctype_file *f;
    char buf[20];
    char *s;
    int var = 12;
    int thing = 0;
    const char** utopia_pointers;

    c_ioq_fmt(ioq2, "Here is the argc: %d\n", argc);

    c_ioq_fmt(ioq2, "Hello from userland!\n");
    
    c_ioq_fmt(ioq2, "First env variable: %s\n", envp[0]);

    c_ioq_fmt(ioq2, "Getting ready to obtain executable path from stack\n");

    for (utopia_pointers = envp; *utopia_pointers != nil; ++utopia_pointers) { }
    ++utopia_pointers;

    c_ioq_fmt(ioq2, "Executable path: %s\n", utopia_pointers[0]);

    c_std_exit(0);
    
}