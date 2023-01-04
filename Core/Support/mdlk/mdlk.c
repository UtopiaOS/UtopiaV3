#include <covenant/std.h>
#include <libmacho/libmacho.h>

uintptr mdlk_main(matype_macho_header *program_header, uintptr program_slide, i32 argc, const char* argv[], const char* envp[], const char* utopia_pointers[]) 
{
    c_ioq_fmt(ioq1, "Hello from mdlk main\n");
    
    // A very important question is, are the necessary segments already mapped? 
    


    c_std_exit(0);   
}