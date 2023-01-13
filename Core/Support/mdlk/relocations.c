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

void mdlk_perform_image_rebase(ImageEntry* entry, RelocationInfo* relocation_info) {
    c_ioq_fmt(ioq1, "Rebase info size: %x\n", relocation_info->rebase_instructions_size);

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
                    return;
                UInt64 segment_offset = 0;
                c_ioq_fmt(ioq1, "Rebase offset: %d\n", segment_offset);
                Status status = mdlk_read_unsigned_leb128(&point, end, &segment_offset);
                if (status != StatusOk)
                    return;
                address = ((UInt64)entry->segments[segment_idx]) + segment_offset;
                break;
            }
            case macho_rebase_opcode_add_address_uleb: {
                c_ioq_fmt(ioq1, "Hit rebase opcode add address uleb\n");
                UInt64 address_offset = 0;
                Status status = mdlk_read_unsigned_leb128(&point, end, &address_offset);
                if (status != StatusOk)
                    return;
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
                    return;
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
                    return;
                address += ammount + sizeof(UIntPtr);
                break;
            }
            case macho_rebase_opcode_perform_rebase_uleb_times_skipping_uleb: {
                UInt64 count = 0;
                UInt64 skip = 0;
                Status status = StatusOk;
                status = mdlk_read_unsigned_leb128(&point, end, &count);
                if (status != StatusOk)
                    return;
                status = mdlk_read_unsigned_leb128(&point, end, &skip);
                if (status != StatusOk)
                    return;
                for (UInt64 i = 0; i < count; ++i) {
                    mdlk_perform_rebase(address, slide, relocation_type);
                    address += skip + sizeof(UIntPtr);
                }
                break;
            }
            default: {
                return;
            }
        }
    }
}

void mdlk_perform_image_bind(ImageEntry* entry, RelocationInfo* relocation_info) {
    const Byte *start = (Byte*)relocation_info->bind_instructions;
    const Byte *end = start + relocation_info->bind_instructions_size;
    const Byte *point = start;

    Bool done = false;

    Int32 segment_idx = 0;

    BindInfo bind_info = {0};

    // Other implementations don't really check for (point < end) which I find weird, that always resulted in a segfault...
    while (!done && (point < end)) {
        Byte immediate = macho_relocation_instruction_get_immediate(*point);
        MachoRebaseOPCode opcode = macho_relocation_instruction_get_opcode(*point);
        ++point;
        
        switch (opcode) {
            case macho_bind_opcode_done:
                done = true;
                break;
            case macho_bind_opcode_set_dylib_ordinal_immediate:
                bind_info.library_ordinal = immediate;
                break;
            case macho_bind_opcode_set_dylib_ordinal_uleb: {
                UIntMax new_library_ordinal = 0;
                Status status = mdlk_read_unsigned_leb128(&point, end, &new_library_ordinal);
                if (status != StatusOk)
                    return;
                bind_info.library_ordinal = new_library_ordinal;
                break;
            }
            case macho_bind_opcode_set_dylib_special_immediate: {
                if (immediate == 0) {
                    bind_info.library_ordinal = immediate;
                } else {
                    SByte sign_extended = 0xf0 | immediate;
                    bind_info.library_ordinal = sign_extended;
                }
                break;
            }
            case macho_bind_opcode_set_symbol_trailing_flags: {
                bind_info.symbol_name = (const char*)point;
                bind_info.flags = immediate;
                while (*point)
                    ++point;
                ++point;
                break;
            }
            case macho_bind_opcode_set_type_immediate:
                bind_info.relocation_type = immediate;
                break;
            case macho_bind_opcode_set_addend_sleb: {
                Int64 addend = 0;
                Status status = mdlk_read_signed_leb128(&point, end, &addend);
                if (status != StatusOk)
                    return;
                bind_info.addend = addend;
                break;
            }
            case macho_bind_opcode_set_segment_immediate_and_offset_uleb: {
                segment_idx = immediate;
                if (((UInt32)segment_idx > entry->number_of_segments || segment_idx < 0)) {
                    return;
                }
                UInt64 offset = 0;
                Status status = mdlk_read_unsigned_leb128(&point, end, &offset);
                if (status != StatusOk)
                    return;
                bind_info.segment_index = segment_idx;
                bind_info.segment_offset = offset;
                break;
            }
            case macho_bind_opcode_add_address_uleb: {
                UInt64 add_address = 0;
                Status status = mdlk_read_unsigned_leb128(&point, end, &add_address);
                if (status != StatusOk)
                    return;
                bind_info.segment_offset += add_address;
                break;
            }
            case macho_bind_opcode_perform_bind:
                c_ioq_fmt(ioq1, "Requested to bind, symbol: %s\n", bind_info.symbol_name);
                break;
            
            default:
                break;
            
        }
    }
}

void mdlk_perform_relocation(ImageEntry* entry, RelocationInfo* relocation_info) {
    mdlk_perform_image_rebase(entry, relocation_info);

    // Now the hard part, perform the binding, or well, try to
    mdlk_perform_image_bind(entry, relocation_info);
}