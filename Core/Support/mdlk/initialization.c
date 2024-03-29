#include <mdlk/mdlk.h>

static UIntPtr slide_of_executable(const matype_macho_header* program_header) {
    const UInt32 command_count = program_header->command_count;
    const matype_load_command* const commands = (matype_load_command*)(((char*)program_header) + sizeof(matype_macho_header));
    const matype_load_command* current_command = commands;
    for (UInt32 i = 0; i < command_count;  i++) {
        if (current_command->type == macho_load_command_type_segment_64) {
            const matype_load_command_segment_64* segment_command = (matype_load_command_segment_64*)current_command;
            if ((segment_command->file_offset == 0) && (segment_command->file_size != 0)) {
                return (UIntPtr)program_header - segment_command->memory_address;
            }
        }
        current_command = (const struct matype_load_command*)(((char*)current_command)+current_command->size);
	}
    return 0;
}  

UIntPtr mdlk_bootstrap_self(const matype_macho_header *program_header, Int32 argc, const char* argv[], IntPtr slide, const matype_macho_header *self_header) 
{
    // In the future we are in fact going to get slided by the kernel, as of now, this wont ever be the case so...
    // TODO: Handle our "self rebase" when we get slided by the kernel
    // (For now, the Utopia kernel makes an exception for dynamic linkers)

    // Lets get our envp and Utopia pointers
    const char** envp = &argv[argc+1];
    const char** utopia_pointers = envp;
    while (*utopia_pointers) { ++utopia_pointers; }
    ++utopia_pointers;
 
    // Get the slide of our program header
    UIntPtr executable_slide = slide_of_executable(program_header);

    // Call our main and get ready to setup the executable
    return mdlk_main(program_header, executable_slide, argc, argv, envp, utopia_pointers);
}