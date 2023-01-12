#include <covenant/std.h>
#include <mdlk/mdlk.h>
#include <covenant/limits.h>
#include <covenant/kernel/basic.h>
#include <covenant/sys/mman.h>
#include <covenant/int.h>
#include <covenant/nix.h>
#include <covenant/vec.h>
#include "internal/_ptrkey.h"


ImageEntry *map_macho_object(MachoHeader* program_header, FileDescriptor fd);


#define	VM_PROT_NONE	((Int32) 0x00)

#define VM_PROT_READ	((Int32) 0x01)	/* read permission */
#define VM_PROT_WRITE	((Int32) 0x02)	/* write permission */
#define VM_PROT_EXECUTE	((Int32) 0x04)	/* execute permission */

static ImageEntry*
image_entry_new() {
    ImageEntry* some_entry = c_std_malloc(sizeof(ImageEntry));
    some_entry->number_of_segments = -1; // So we actually hit 0, TODO: Is this hacky or not?
    some_entry->symbol_table = nil;
    some_entry->string_table = nil;
    some_entry->symbol_table_entry_count = 0;
    some_entry->string_table_size = 0;
    c_vec_init(&some_entry->dependencies, 2, sizeof(UIntPtr));
    c_vec_init(&some_entry->dependants, 2, sizeof(UIntPtr));
    return some_entry;
}

static Int32
image_entry_dependency_cmp(UniversalType a, UniversalType b) {
    ImageEntryDependency* ia = (ImageEntryDependency*)a;
    ImageEntryDependency* ib = (ImageEntryDependency*)b;
    return 0;
    //return c_str_cmp(ia->name, C_USIZE_MAX, ib->name);
}

MachoHeader* mdlk_get_macho_header_by_path(char* some_path) {
    FileDescriptor fd = c_kernel_open2(some_path, C_NIX_OREAD);
    if (fd < 0)
        goto err;
    UniversalType status = c_kernel_mmap(nil, 4 * C_PAGESIZE, C_PROT_READ, C_MAP_PRIVATE, fd, 0);
    if (status == C_MAP_FAILED)
        goto err;
    return (MachoHeader*)status;
err:
    c_kernel_munmap(status, 4 * C_PAGESIZE);
    c_kernel_close(fd);
    return nil;
}

ImageEntry* mdlk_load_by_path(char* some_path) {
    MachoHeader* header = mdlk_get_macho_header_by_path(some_path);
    if (!header) {
        c_ioq_fmt(ioq1,"Error getting the header!");
        return nil;
    }
    FileDescriptor fd = c_kernel_open2(some_path, C_NIX_OREAD);
    return map_macho_object(header, fd);
}

static Int32 macho_flags_to_mmap_flags(Int32 flags) {
    Int32 new_flags = C_MAP_PRIVATE;
    return new_flags; // We don't have C_MAP_NOCORE on Linux :/
}

static Int32 macho_flags_to_mmap_protection(Int32 flags) {
    Int32 current_prot = 0;
    if (flags & VM_PROT_READ)
        current_prot |= C_PROT_READ;
    if (flags & VM_PROT_WRITE)
        current_prot |= C_PROT_WRITE;
    if (flags & VM_PROT_EXECUTE)
        current_prot |= C_PROT_EXEC;
    return current_prot;
}

ImageEntry *map_macho_object(MachoHeader* program_header, FileDescriptor fd) {
    Macho64Addr offset = 0;
    MachoLoadCommandDylib* some_dylib = nil;
    void* file_load_top = nil;
    Macho64Addr base_virtual_address;
    Macho64Addr base_virtual_limit;

    Macho64Off data_offset;
    Macho64Addr data_virtual_address;
    Macho64Addr data_virtual_limit;
    UniversalType data_address;

    Macho64Addr main_address = 0; // Save it before we store it in the actual data structure with our reloc offset

    // Displace the header, we already have it mapped
    offset = (Macho64Addr)program_header;
    offset += sizeof(MachoHeader);

    Int32 base_flags;

    ImageEntry *current_image = image_entry_new();
    UniversalType base_address = nil;

    USize map_size = 0;

    // Find:
    // 1. How big is the area we plan on mapping
    // 2. What is our entry point
    // 3. Symbol table offset
    // 4. String table offset
    for (UInt32 i = 0; i < program_header->command_count; i++) {
        MachoLoadCommand* some_lc = (MachoLoadCommand*)offset;

        switch (some_lc->type) {
            case macho_load_command_type_segment_64: {
                MachoLoadCommandSegment64* some_segment_64 = (MachoLoadCommandSegment64*)some_lc;
                current_image->number_of_sections += some_segment_64->section_count;
                current_image->segments[++current_image->number_of_segments]=some_segment_64;
                break;
            }
            case macho_load_command_type_entry_point: {
                MachoLoadCommandEntryPoint* some_entry_point = (MachoLoadCommandEntryPoint*)some_lc;
                main_address = some_entry_point->entry_offset;
                break;
            }
            case macho_load_command_type_symbol_table_info: { //! THESE AREN'T RELOCATED YET, THEY ARE RELOCATED AFTER WE HAVE OUR BASE ADDR
                MachoLoadCommandSymbolTableInfo* some_symbol_table_info = (MachoLoadCommandSymbolTableInfo*)some_lc;
                current_image->symbol_table = (Macho64Symbol*) some_symbol_table_info->symbol_table_offset;
                current_image->symbol_table_entry_count = some_symbol_table_info->symbol_table_entry_count;
                current_image->string_table = (const char*) some_symbol_table_info->string_table_offset;
                current_image->string_table_size = some_symbol_table_info->string_table_size;
                break;
            }
            case macho_load_command_type_load_dynamic_linker: // This is us
            default:
                break;
        }
        offset += some_lc->size;
    }
    base_virtual_address = trunc_page(current_image->segments[0]->memory_address);
    base_virtual_limit = round_page(current_image->segments[current_image->number_of_segments]->memory_address + current_image->segments[current_image->number_of_segments]->memory_size);
    map_size = base_virtual_limit - base_virtual_address - current_image->segments[0]->memory_size + C_PAGESIZE;
    base_address = (UniversalType)base_virtual_address;

    base_flags = C_MAP_PRIVATE | C_MAP_ANON;
    void* map_base = c_kernel_mmap(base_address, map_size, C_PROT_NONE, base_flags, -1, 0);
    if (map_base == C_MAP_FAILED) {
        c_ioq_fmt(ioq1, "Unable to map\n");
        return nil;
    }

    // Since our base virtual address might be 0, when we typecast to UniversalType, we get the following underlying code:
    // base_address = (UniversalType)base_virtual_address = (void*)0 = nil
    // so mmap, interprets it as "load it anywhere", and we would error if we only did the map_base and base_address comparison
    if (base_address != nil && map_base != base_address) {
        c_ioq_fmt(ioq1, "Failed to get desired address\n");
        return nil;
    }

    // Is time to overlay our segment commands
    for (Int32 i = 0; i <= current_image->number_of_segments; i++) {
        if (c_str_equal(current_image->segments[i]->segment_name, C_USIZE_MAX, "__PAGEZERO"))
            continue;
        data_offset = trunc_page(current_image->segments[i]->file_offset);
        data_virtual_address = trunc_page(current_image->segments[i]->memory_address - current_image->segments[0]->memory_size);
        data_virtual_limit = round_page(current_image->segments[i]->memory_address - current_image->segments[0]->memory_size + current_image->segments[i]->file_size);
        data_address = map_base + (data_virtual_address - base_virtual_address);
        Int32 protection_flags = macho_flags_to_mmap_protection(current_image->segments[i]->initial_memory_protection);
        Int32 data_flags = macho_flags_to_mmap_flags(current_image->segments[i]->initial_memory_protection) | C_MAP_FIXED;
        UniversalType res = c_kernel_mmap(data_address, data_virtual_limit - data_virtual_address, protection_flags, data_flags, fd, data_offset);
        if (res == C_MAP_FAILED) {
            c_ioq_fmt(ioq1, "Failed to overlay segments\n");
            return nil;
        }
    }


    // Actually fill up some information for the image now
    current_image->map_base = map_base;
    current_image->map_size = map_size;

    current_image->virtual_addr_base = base_virtual_address;
    current_image->relocation_base = map_base - base_virtual_address;

    // Fix up addresses based on our offset given by our relocation base
    if (current_image->symbol_table)
        current_image->symbol_table = (Macho64Addr* )(((PtrDiff)current_image->relocation_base) + ((PtrDiff)current_image->symbol_table));
    if (current_image->string_table)
        current_image->string_table = (const char*)(((PtrDiff)current_image->relocation_base) + ((PtrDiff)current_image->string_table));
    
    // Fix up file offset again, as we plan on iterating from the header to find our dylib load commands    
    offset = (Macho64Addr)program_header;
    offset += sizeof(MachoHeader);
    // Find dylibs we are going to append to our own vector
    for (Int32 i=0; i < program_header->command_count; i++) {
        MachoLoadCommand* some_lc = (MachoLoadCommand*)offset;

        if (some_lc->type != macho_load_command_type_load_dylib) {
            offset += some_lc->size;
            continue;
        }

        // We found one, prepare to get the path and pass it to our other function (That will get the correspoding Mach-O header and then call this function again)
        MachoLoadCommandDylib* some_dylib = (MachoLoadCommandDylib*)some_lc;
        char* dylib_path = (char*)some_lc + some_dylib->name_offset; // The offset in this case, is relative to the current segment
        c_ioq_fmt(ioq1, "%s\n", dylib_path);
        ImageEntry* dependency = mdlk_load_by_path(dylib_path);
        c_vec_push_back(&current_image->dependencies, dependency); // Register it as a dependency 
        c_vec_push_back(&dependency->dependants, current_image); // Register the dependants
        offset += some_lc->size;
    }

    return current_image;
}

struct MDLKGlobalState mdlk_global_state;

// Return the main address of the program after everything is ready
UIntPtr mdlk_main(MachoHeader *program_header, UIntPtr program_slide, UInt32 argc, const char* argv[], const char* envp[], const char* utopia_pointers[]) 
{    
    mdlk_global_state.dependency_graph = c_std_malloc(sizeof(TailQueue));
    c_tq_init(mdlk_global_state.dependency_graph, sizeof(ImageEntryDependency), &image_entry_dependency_cmp, nil);
    
    FileDescriptor fd = c_kernel_open2(mdlk_get_executable_path(utopia_pointers[0]), C_NIX_OREAD);

    map_macho_object(program_header, fd);  
    c_std_exit(0);   
}