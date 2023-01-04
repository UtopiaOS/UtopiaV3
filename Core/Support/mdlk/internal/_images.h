
#ifndef MDLK_INTERNAL_IMAGES_H
#define MDLK_INTERNAL_IMAGES_H

#include <covenant/gdat.h>
#define __NEED_size
#define __NEED_i32
#include <covenant/bits/everytype.h>
#include <covenant/bool.h>
#include <covenant/dat.h>


typedef struct mdlk_image mdtype_mdlk_image;
typedef struct mdlk_section mdtype_mdlk_section;
typedef struct mdlk_segment mdtype_mdlk_segment;

struct mdlk_section {
    char section_name[16];
    char segment_name[16];
    void *address;
    Size total_size;
    Size file_offset;
};

struct mdlk_segment {
    char name[16];
    void *address;
    Size total_size;
};

struct mdlk_image {
    const char* path;
    Size path_lenght;

    const char* name; // Is C string ok or shoud we use our strings?
    Size name_lenght;

    // We don't have a file implementation yet!!
    ctype_file* file;
    void *entry_address;
    void *file_load_base;
    Size total_size;

    mdtype_mdlk_section* sections;
    Size section_count;

    mdtype_mdlk_segment* segments;
    Size segment_count;

    HashMap exports_table;

    Size dependency_count;
    mdtype_mdlk_image** dependencies; // Recursivness is a pain....

    Size dependant_count;
    mdtype_mdlk_image** dependents;

    void* export_trie;
    Size export_trie_size;

    void* lazy_bind_instructions;
    Size lazy_bind_instructions_size;

    bool is_libmdlk;
};

#endif // MDLK_INTERNAL_IMAGES_H