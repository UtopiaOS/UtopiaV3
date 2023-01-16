#include <mdlk/mdlk.h>
#include "internal/_leb128.h"
#include <covenant/limits.h>

typedef struct BindInfo BindInfo;

struct BindInfo {
    MachoRelocationType relocation_type;
    UInt32 segment_index;
    USize segment_offset;
    const char* symbol_name;
    USize symbol_lenght;
    UIntMax library_ordinal;
    Int64 addend; // TODO: Make this IntMax
    Byte flags;
};


static void
mdlk_perform_rebase(UInt64 address, UInt64 slide, MachoRelocationType type) {
    UInt64 val = 0;
    switch (type) {
        case macho_relocation_type_pointer:
        case macho_relocation_type_text_absolute_32:
            val = *(UInt64 *)address;
            c_ioq_fmt(ioq1, "Address: %p, val: %p, rebase: %p\n", (UniversalType)address, (UniversalType)val, (UniversalType)val + slide);
            *(UInt64 *)address = val + slide;
            break;
        default:
            break; // Don't really care, well, in theory we DO care, but not as of now, in the future let the caller know it is an invalid arg
    }
}

static void
mdlk_perform_bind(ImageEntry* entry, BindInfo* bind_info) {
    UniversalType address = (char*)entry->segments[bind_info->segment_index] + bind_info->segment_offset; // Since we mmaped, we already have the segment address
    ImageSymbol* symbol_to_bind = nil;
    
    mdlk_resolve_export(entry, bind_info->symbol_name, bind_info->symbol_lenght, &symbol_to_bind);
}

Status mdlk_perform_image_rebase(ImageEntry* entry, RelocationInfo* relocation_info) {
    c_ioq_fmt(ioq1, "Performing rebase for: %s with rebase size of: %d\n", entry->path, relocation_info->rebase_instructions_size);
    
    c_ioq_fmt(ioq1, "Currently at file offset: %p\n", (UniversalType)(((PtrDiff)relocation_info->rebase_instrucions) - ((PtrDiff)entry->relocation_base)));
    
    UInt64 address = 0;
    MachoRelocationType relocation_type = macho_relocation_type_pointer;
    Int32 segment_idx = 0;

    UInt64 slide = ((UInt64)entry->relocation_base) - 0x100000000;

    const Byte *start = (Byte*)relocation_info->rebase_instrucions;
    const Byte *end = start + relocation_info->rebase_instructions_size;
    const Byte *point = start;

    Bool done = false;
    
    
    while (!done && (point < end)) {
        Byte immediate = macho_relocation_instruction_get_immediate(*point);
        MachoRebaseOPCode opcode = macho_relocation_instruction_get_opcode(*point);
        ++point;
    
        switch (opcode) {
            case macho_rebase_opcode_done:
                done = true;
                break;
            case macho_rebase_opcode_set_type_immediate:
                c_ioq_fmt(ioq1, "Setting rebase opcode to immediate\n");
                relocation_type = immediate;
                break;
            case macho_rebase_opcode_set_segment_immediate_and_offset_uleb: {
                segment_idx = immediate;
                c_ioq_fmt(ioq1, "Setting segment to immediate and offsetting uleb. Segment idx: %d\n", segment_idx);
                if (((UInt32)segment_idx > entry->number_of_segments || segment_idx < 0))
                    return StatusErr;
                UInt64 segment_offset = 0;
                c_ioq_fmt(ioq1, "Rebase offset: %d\n", segment_offset);
                Status status = mdlk_read_unsigned_leb128(&point, end, &segment_offset);
                if (status != StatusOk)
                    return status;
                address = ((UInt64)entry->segments[segment_idx]) + segment_offset;
                break;
            }
            case macho_rebase_opcode_add_address_uleb: {
                c_ioq_fmt(ioq1, "Hit rebase opcode add address uleb\n");
                UInt64 address_offset = 0;
                Status status = mdlk_read_unsigned_leb128(&point, end, &address_offset);
                if (status != StatusOk)
                    return status;
                address += address_offset;
                break;
            }
            case macho_rebase_opcode_add_immediate_scaled:
                address += immediate * sizeof(UIntPtr);
                break;
            case macho_rebase_opcode_perform_rebase_immediate_times: {
                c_ioq_fmt(ioq1, "Immediate: %d\n", immediate);
                for (Int32 i = 0; i < immediate; ++i) {
                    mdlk_perform_rebase(address, slide, relocation_type);
                    address += sizeof(UIntPtr);
                }
                break;
            }
            case macho_rebase_opcode_perform_rebase_uleb_times: {
                UInt64 count = 0;
                Status status = mdlk_read_unsigned_leb128(&point, end, &count);
                if (status != StatusOk)
                    return status;
                for (UInt64 i = 0; i < count; ++i) {
                    mdlk_perform_rebase(address, slide, relocation_type);
                    address += sizeof(UIntPtr);
                }
                break;
            }
            case macho_rebase_opcode_perform_rebase_add_uleb: {
                c_ioq_fmt(ioq1, "Hit rebase opcode perform rebase add uleb\n");
                mdlk_perform_rebase(address, slide, relocation_type);
                UInt64 ammount = 0;
                Status status = mdlk_read_unsigned_leb128(&point, end, &ammount);
                if (status != StatusOk)
                    return status;
                address += ammount + sizeof(UIntPtr);
                break;
            }
            case macho_rebase_opcode_perform_rebase_uleb_times_skipping_uleb: {
                UInt64 count = 0;
                UInt64 skip = 0;
                Status status = StatusOk;
                status = mdlk_read_unsigned_leb128(&point, end, &count);
                if (status != StatusOk)
                    return status;
                status = mdlk_read_unsigned_leb128(&point, end, &skip);
                if (status != StatusOk)
                    return status;
                for (UInt64 i = 0; i < count; ++i) {
                    mdlk_perform_rebase(address, slide, relocation_type);
                    address += skip + sizeof(UIntPtr);
                }
                break;
            }
            default: {
                return StatusOk;
            }
        }
    }

    return StatusOk;
}

Status mdlk_perform_image_bind(ImageEntry* entry, RelocationInfo* relocation_info) {
    const Byte *start = relocation_info->bind_instructions;
    const Byte *end = start + relocation_info->bind_instructions_size;
    const Byte *point = start;

    for (USize i = 0; i < relocation_info->bind_instructions_size; i++) {
        c_ioq_fmt(ioq1, " %x", ((Byte*)relocation_info->bind_instructions)[i]);
    }


    Bool done = false;

    Int32 segment_idx = 0;

    BindInfo bind_info = {0};

    c_ioq_fmt(ioq1, "Performing binding for image: %s, with bind instruction size of: %d\n", entry->path, relocation_info->bind_instructions_size);

    // Other implementations don't really check for (point < end) which I find weird, that always resulted in a segfault...
    while (!done && (point < end)) {
        c_ioq_fmt(ioq1, "Current file offset: %p\n", (UniversalType)(((PtrDiff)(point)) - ((PtrDiff)(entry->relocation_base))));
        Byte immediate = macho_relocation_instruction_get_immediate(*point);
        MachoBindCode opcode = macho_relocation_instruction_get_opcode(*point);
        ++point;

        c_ioq_fmt(ioq1, "Point and end difference: %d\n", (Int32)(end - point));
        c_ioq_fmt(ioq1, "OPCode: %d\n", opcode);
        
        switch (opcode) {
            case macho_bind_opcode_done: {
                done = true;
                break;
            }
            case macho_bind_opcode_set_dylib_ordinal_immediate: {
                c_ioq_fmt(ioq1, "macho_bind_opcode_set_dylib_ordinal_immediate\n");
                bind_info.library_ordinal = immediate;
                break;
            }
            case macho_bind_opcode_set_dylib_ordinal_uleb: {
                UInt64 new_library_ordinal = 0;
                Status status = mdlk_read_unsigned_leb128(&point, end, &new_library_ordinal);
                if (status != StatusOk)
                    return status;
                bind_info.library_ordinal = new_library_ordinal;
                break;
                c_ioq_fmt(ioq1, "macho_bind_opcode_set_dylib_ordinal_uleb\n");
            }
            case macho_bind_opcode_set_dylib_special_immediate: {
                c_ioq_fmt(ioq1, "macho_bind_opcode_set_dylib_special_immediate\n");
                if (immediate == 0) {
                    bind_info.library_ordinal = immediate;
                } else {
                    SByte sign_extended = 0xf0 | immediate;
                    bind_info.library_ordinal = sign_extended;
                }
                break;
            }
            case macho_bind_opcode_set_symbol_trailing_flags: {
                c_ioq_fmt(ioq1, "macho_bind_opcode_set_symbol_trailing_flags\n");
                bind_info.symbol_name = (const char*)point;
                bind_info.symbol_lenght = c_str_len(bind_info.symbol_name, C_USIZE_MAX);
                bind_info.flags = immediate;
                while (*point)
                    ++point;
                ++point;
                break;
            }
            case macho_bind_opcode_set_type_immediate: {
                c_ioq_fmt(ioq1, "macho_bind_opcode_set_type_immediate\n");
                bind_info.relocation_type = immediate;
                break;
            }
            case macho_bind_opcode_set_addend_sleb: {
                Int64 addend = 0;
                Status status = mdlk_read_signed_leb128(&point, end, &addend);
                if (status != StatusOk)
                    return status;
                bind_info.addend = addend;
                break;
            }
            case macho_bind_opcode_set_segment_immediate_and_offset_uleb: {
                segment_idx = immediate;
                if (((UInt32)segment_idx > entry->number_of_segments || segment_idx < 0)) {
                    return StatusErr;
                }
                UInt64 offset = 0;
                Status status = mdlk_read_unsigned_leb128(&point, end, &offset);
                if (status != StatusOk)
                    return status;
                bind_info.segment_index = segment_idx;
                bind_info.segment_offset = offset;
                c_ioq_fmt(ioq1, "macho_bind_opcode_set_segment_immediate_and_offset_uleb\n");
                break;
            }
            case macho_bind_opcode_add_address_uleb: {
                UInt64 add_address = 0;
                Status status = mdlk_read_unsigned_leb128(&point, end, &add_address);
                if (status != StatusOk)
                    return status;
                bind_info.segment_offset += add_address;
                break;
            }
            case macho_bind_opcode_perform_bind:
                c_ioq_fmt(ioq1, "Requested to bind, symbol: %s\n", bind_info.symbol_name);
                mdlk_perform_bind(entry, &bind_info);
                bind_info.segment_offset += sizeof(UIntPtr);
                break;
            case macho_bind_opcode_perform_bind_add_address_uleb: {
                UInt64 add_address = 0;
                mdlk_perform_bind(entry, &bind_info);
                bind_info.segment_offset += sizeof(UIntPtr);
                Status status = mdlk_read_unsigned_leb128(&point, end, &add_address);
                if (status != StatusOk)
                    return status;
                bind_info.segment_offset += add_address;
                break;
            }
            case macho_bind_opcode_perform_bind_add_address_immediate_scaled: {
                mdlk_perform_bind(entry, &bind_info);
                bind_info.segment_offset += immediate * sizeof(UIntPtr) + sizeof(UIntPtr);
                break;
            }
            case macho_bind_opcode_perform_bind_uleb_times_skipping_uleb: {
                c_ioq_fmt(ioq1, "Here, perform and skip uleb\n");
                UInt64 count = 0;
                UInt64 skip = 0;
                Status status = StatusOk;
                status = mdlk_read_unsigned_leb128(&point, end, &count);
                if (status != StatusOk)
                    return status;
                status = mdlk_read_unsigned_leb128(&point, end, &skip);
                if (status != StatusOk)
                    return status;
                for (UInt64 i = 0; i < count; ++i) {
                    mdlk_perform_bind(entry, &bind_info);
                    bind_info.segment_offset += skip + sizeof(UIntPtr);
                }
                break;
            }
            default:
                c_ioq_fmt(ioq1, "Unkown opcode as of now (this is literally why it isn't relocating properly)\n");
                return StatusErr;
        }
    }

    return StatusOk;
}

void mdlk_perform_relocation(ImageEntry* entry, RelocationInfo* relocation_info) {
    Status status = StatusOk;

    status = mdlk_perform_image_rebase(entry, relocation_info);
    if (status != StatusOk)
        c_ioq_fmt(ioq1, "Failed to rebase %s\n", entry->path);

    // Now the hard part, perform the binding, or well, try to
    //status = mdlk_perform_image_bind(entry, relocation_info);
    //if (status != StatusOk)
    //    c_ioq_fmt(ioq1, "Failed to bind %s\n", entry->path);
}