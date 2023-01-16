#include <mdlk/mdlk.h>
#include <covenant/hm.h>
#include <covenant/limits.h>
#include "internal/_leb128.h"

static Bool check_if_prefix_and_output_length(const char* first, const char* second, USize second_length, USize* out_first_length) {
    USize i = 0;
    USize first_length = 0;
    Bool result = true;
    
    for (; i < second_length; ++i) {
        if (first[i] == '\0') {
            break;
        } else if(first[i] != second[i]) {
            result = false;
            break;
        }
    }

    first_length = i + c_str_len(&first[i], C_USIZE_MAX);

    if (first_length > second_length)
        result = false;
    
    if (out_first_length)
        *out_first_length = first_length;
    
    return result;
}

static void mdlk_export_trie_lookup(ImageEntry* entry, const char* name, USize name_length, const UniversalType* out_ptr) {
    c_ioq_fmt(ioq1, "Looking for %s in %s\n", name, entry->path); // TODO: Image entry SHOULD always get a path name
    const Byte *start = (Byte*)entry->export_trie;
    const Byte *end = start + entry->export_trie_size;
    const Byte *point = start;
    USize name_offset = 0;
    UniversalType pointer = nil;

    while (point < end) {
        UInt64 export_info_size = 0;
        Byte child_count = 0;
        Bool found_child = false;

        Status status = mdlk_read_unsigned_leb128(&point, end, &export_info_size);
        c_ioq_fmt(ioq1, "Export info size: %d\n", export_info_size);

        if (status != StatusOk)
            return;
        
        if (export_info_size != 0 && name_offset == name_length) {
            pointer = &point;
            goto out; 
        }

        point += export_info_size;

        c_ioq_fmt(ioq1, "Current offset: %p\n", (UniversalType)(((PtrDiff)point) - ((PtrDiff)entry->relocation_base)));

        child_count = *point;
        ++point; // ?

        c_ioq_fmt(ioq1, "Current offset: %p\n", (UniversalType)(((PtrDiff)point) - ((PtrDiff)entry->relocation_base)));

        c_ioq_fmt(ioq1, "Child count = %d\n", child_count);
        
        for (Byte i = 0; i < child_count; ++i) {
            c_ioq_fmt(ioq1, "Current offset: %p\n", (UniversalType)(((PtrDiff)point) - ((PtrDiff)entry->relocation_base)));
            USize child_name_length = 0;
            Bool correct_child = check_if_prefix_and_output_length(point, &name[name_offset], name_length - name_offset, &child_name_length);
            c_ioq_fmt(ioq1, "Checking %s with %s = %s\n", point, &name[name_offset], correct_child ? "true" : "false");
            UInt64 child_offset = 0;

            while (*point)
                ++point;
            ++point;

            Status status = mdlk_read_unsigned_leb128(&point, end, &child_offset);
            c_ioq_fmt(ioq1, "Child offset: %d\n", child_offset);
            if (status != StatusOk)
                return;

            if (!correct_child)
                continue;
        }
    }
out:
    return;
}


void mdlk_resolve_export(ImageEntry* entry, const char* name, USize name_length, ImageSymbol** out_symbol) {
    // Let's see if we already have it in our table
    c_ioq_fmt(ioq1, "Image %s has requested a lookup for %s\n", entry->path, name);
    ImageSymbol* some_symbol = c_hm_get(&entry->exports_table, &(ImageSymbol){.name = name});
    if (some_symbol) {
        c_ioq_fmt(ioq1, "Symbol %s was found in the export table of %s\n", name, entry->path);
        goto out;
    }

    c_ioq_fmt(ioq1, "%d\n", name_length);
    // Let's try to actually find it in the trie now
    mdlk_export_trie_lookup(entry, name, name_length, nil);

out:
    return;
}