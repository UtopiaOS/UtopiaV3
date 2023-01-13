/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright 1996, 1997, 1998, 1999, 2000 John D. Polstra.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $FreeBSD$
 */

/* Heaviliy modified version. Diego Magdaleno 2022
 * This version was modified for Utopia, which uses Mach-O as it's main file format
 * This version is quite different in the sense that it tries to remove the "Elf" references (The file formats aren't that different to the linker)
 * The initial modifications to load Mach-O files were performed by: https://github.com/mszoek
 * 
 * This version relies on Utopia types, instead of the standard definitons. 
*/

// TODO: As for the linked list, we might move to our "libGeneric" linked list sometime in the future

#ifndef MDLK_H
#define MDLK_H

#define __NEED_Int32
#define __NEED_Size
#define __NEED_UIntPtr
#define __NEED_Device
#define __NEED_IntPtr
#define __NEED_PtrDiff
#include <covenant/bits/everytype.h>
#include <covenant/dat.h>
#include <libmacho/libmacho.h>
#include <covenant/tq.h>
#include <covenant/vec.h>

/*
extern size_t tls_last_offset;
extern size_t tls_last_size;
extern size_t tls_static_space;
extern Elf_Addr tls_dtv_generation;
extern int tls_max_index;

* Note: Last time I researched about tls it turns out these are completely differnt on Mach-O and ELF? So for now, this is staying commented as reference
*/

extern Int32 npagesizes;
extern Size *pagesizes;

extern Int32 main_argc;
extern char **main_argv;
extern char **environ; // or envp
// TODO: We might need Utopia pointers

// Taken from NetBSD
#define trunc_page(x) ((x) & ~(C_PAGESIZE - 1))
#define round_page(x) (((x) + C_PAGESIZE - 1) & ~(C_PAGESIZE - 1))

/* Types of main functions */
typedef void (*InitFunction)(void);
typedef void (*InitWithArgumentsFunction)(Int32, char**, char**);


// In Apple's dyld, Image is used as an "object" synonym
typedef struct ImageEntry ImageEntry;
typedef struct ImageEntryDependency ImageEntryDependency;
typedef struct RelocationInfo RelocationInfo;


struct MDLKGlobalState {
    TailQueue* dependency_graph;
};

extern struct MDLKGlobalState mdlk_global_state;

struct ImageEntryDependency {
    ULong name; // Offset to name in the LC_LOAD_DYLIB
    ImageEntry* image_object; // The object it is referencing, at the start, nil
};

struct RelocationInfo {
    UniversalType rebase_instrucions;
    USize rebase_instructions_size;

    UniversalType bind_instructions;
    USize bind_instructions_size;

    UniversalType weak_bind_instructions;
    USize weak_bind_instructions_size;
};

struct ImageEntry {
    UInt64 *lazy_loaded_symbols; // TODO: Maybe this is addr type?

    Device device; // Related device
    INode inode; // Inode of object

    UniversalType map_base; // This used caddr_t, that is some legacy remainings from BSD. Represents the base address of the mapped region.
    Size map_size; // Size of the mapped region in bytes

    Size text_size; // Size of the text size in bytes

    Macho64Addr virtual_addr_base; // Base address in a shared object file
    UniversalType relocation_base; // Relocation constant = map_base - virtual_addr_base

    const char* interpreter; // Path name of the interpreter, if any

    Macho64Word stack_flags; // Well, stack flags

    /* Items that belong to dynamic section */
    Macho64Addr *pltgot;

    /* String table */
    const char* string_table;
    ULong string_table_size;

    /* Symbol table */
    const Macho64Symbol *symbol_table;
    UInt32 symbol_table_entry_count;

    /* Adress of our entry point (main) */
    UniversalType entry;

    Bool is_mdlk : 1; // Wether we are the dynamic linker or not, we plan to use only 1 bit of this...

    UInt32 number_of_sections; // Current number of sections per segment

    UInt32 number_of_segments; // Current number of segments
    UniversalType segments[16]; // Don't quote me on 16: https://github.com/ravynsoft/ravynos/blob/610d6be060509eea727d87d315267c677e6eaf00/libexec/dyld/map_object.c#L373

    UniversalType relocated_segments; // Segments address after performing a relocation
    UniversalType linkedit_ptr; // Pointer to our LINKEDIT section

    // Export trie section
    UniversalType export_trie;
    USize export_trie_size;

    // Type: ImageEntry
    Vector dependencies;
    Vector dependants;
};

// TODO: Move this out of here
void mdlk_perform_relocation(ImageEntry*, RelocationInfo*);

#endif