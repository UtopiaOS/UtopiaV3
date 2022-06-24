#include "libC/internal/attributes.h"

#define __NEED_i32
#define __NEED_u32
#define __NEED_u64
#define __NEED_u16
#define __NEED_umax
#define __NEED_u8
#include <covenant/bits/everytype.h>
#include <covenant/bool.h>

UTOPIA_ENUM(i32, cpu_type, ma){
    macho_cpu_type_x86_64 = 0x01000007,
};

UTOPIA_ENUM(i32, cpu_subtype, ma){
    macho_cpu_type_x86_64_all = 3,
};

UTOPIA_ENUM(u32, file_type, ma){
    macho_file_type_object = 1,
    macho_file_type_executable = 2,
    macho_file_type_dynamic_library = 6,
    macho_file_type_dynamic_linker = 7,
};

UTOPIA_OPTIONS(u32, header_flags, ma)
{
    macho_header_flag_no_undefined_symbols = 1ULL << 0,
    macho_header_flag_dynamically_linked = 1ULL << 2,
    macho_header_flag_use_two_level_namespace = 1ULL << 7,
    macho_header_flag_pie = 1ULL << 21,
};

UTOPIA_PACKED_STRUCT(macho_header, ma) {
    u32 magic;
    matype_cpu_type cpu_type;
    matype_cpu_subtype cpu_subtype;
    matype_file_type file_type;
    u32 command_count;
    u32 total_command_size;
    matype_header_flags flags;
    u32 reserved;
    char load_commands[];
};

#define MACHO_MAGIC_64 (0xfeedfacfU)

UTOPIA_ENUM(u32, load_command_type, ma) {
    macho_load_command_type_symbol_table_info = 0x02,
    macho_load_command_type_unix_thread = 0x05,
    macho_load_command_type_dynamic_symbol_table_info = 0x0b,
    macho_load_command_type_load_dylib = 0x0c,
    macho_load_command_type_load_dynamic_linker = 0x0e,
    macho_load_command_type_segment_64 = 0x19,
    macho_load_command_type_compressed_dynamic_linker_info_only = 0x80000022,
    macho_load_command_type_entry_point = 0x80000028, // ?
};

UTOPIA_PACKED_STRUCT(load_command, ma) {
    matype_load_command_type type;
    u32 size;
};


UTOPIA_PACKED_STRUCT(load_command_segment_64, ma) {
    matype_load_command base;
    char segment_name[16];
    u64 memory_address;
    u64 memory_size;
    u64 file_offset;
    u64 file_size;
    i32 maximum_memory_protection;
    i32 initial_memory_protection;
    u32 section_count;
    u32 flags;
};

UTOPIA_PACKED_STRUCT(section_64, ma) {
    char section_name[16];
    char segment_name[16];
    u64 memory_address;
    u64 size;
    u32 file_offset;
    u32 alignment;
    u32 relocations_file_offset;
    u32 relocation_count;
    u32 flags;
    u32 reserved1;
    u32 reserved2;
    u32 reserved3;
};

UTOPIA_PACKED_STRUCT(load_command_thread, ma) {
    matype_load_command base;
};

UTOPIA_PACKED_STRUCT(load_command_dynamic_linker, ma) {
    matype_load_command base;
    u32 name_offset;
};

UTOPIA_PACKED_STRUCT(load_command_dylib, ma) {
    matype_load_command base;
    u32 name_offset;
    u32 timestamp;
    u32 current_version;
    u32 compat_version;
};

UTOPIA_PACKED_STRUCT(load_command_symbol_table_info, ma) {
    matype_load_command base;

    u32 symbol_table_offset;
    u32 symbol_table_entry_count;

    u32 string_table_offset;
    u32 string_table_size;
};

UTOPIA_PACKED_STRUCT(load_command_dynamic_symbol_table_info, ma) {
	matype_load_command base;

	u32 local_symbols_start_index;
	u32 local_symbol_count;

	u32 external_symbols_start_index;
	u32 external_symbol_count;

	u32 undefined_symbols_start_index;
	u32 undefined_symbol_count;

	u32 table_of_contents_offset;
	u32 table_of_contents_entry_count;

	u32 module_table_offset;
	u32 module_table_entry_count;

	u32 external_reference_table_offset;
	u32 external_reference_table_entry_count;

	u32 indirect_symbol_table_offset;
	u32 indirect_symbol_table_entry_count;

	u32 external_relocations_offset;
	u32 external_relocation_count;

	u32 local_relocations_offset;
	u32 local_relocation_count;
};

UTOPIA_ENUM(u8, symbol_table_entry_type, ma) {
	macho_symbol_table_entry_type_undefined = 0,
	macho_symbol_table_entry_type_absolute  = 1,
	macho_symbol_table_entry_type_indirect  = 5,
	macho_symbol_table_entry_type_prebound  = 6,
	macho_symbol_table_entry_type_section   = 7,
};

UTOPIA_ENUM(u8, symbol_table_entry_section, ma) {
	macho_symbol_table_entry_section_none = 0,
	// all other values are valid section indicies
};

UTOPIA_ENUM(u8, symbol_table_entry_library_index, ma) {
	macho_symbol_table_entry_library_index_self           = 0,
	macho_symbol_table_entry_library_index_dynamic_lookup = 0xfe,
	macho_symbol_table_entry_library_index_executable     = 0xff,
};

UTOPIA_PACKED_STRUCT(symbol_table_entry, ma) {
	u32 string_table_name_offset;
	u8 type;
	u8 section;
	u16 description;
	u64 value;
};

UTOPIA_ALWAYS_INLINE matype_symbol_table_entry_type macho_symbol_table_entry_get_type(u8 type_field) {
	return (type_field >> 1) & 7;
};

UTOPIA_ALWAYS_INLINE bool macho_symbol_table_entry_is_external(u8 type_field) {
	return type_field & 1;
};

UTOPIA_ALWAYS_INLINE bool macho_symbol_table_entry_is_private_extern(u8 type_field) {
	return (type_field >> 4) & 1;
};

UTOPIA_ALWAYS_INLINE u8 macho_symbol_table_entry_library_index(u16 description_field) {
	return description_field >> 8;
};

UTOPIA_ALWAYS_INLINE bool macho_symbol_table_entry_library_index_is_special(u8 library_index) {
	switch (library_index) {
		case macho_symbol_table_entry_library_index_self:
		case macho_symbol_table_entry_library_index_dynamic_lookup:
		case macho_symbol_table_entry_library_index_executable:
			return true;
		default:
			return false;
	}
};

UTOPIA_PACKED_STRUCT(load_command_entry_point, ma) {
	matype_load_command base;

	u64 entry_offset;
	u64 stack_size;
};

UTOPIA_PACKED_STRUCT(load_command_compressed_dynamic_linker_info, ma) {
	matype_load_command base;

	u32 rebase_info_offset;
	u32 rebase_info_size;

	u32 bind_info_offset;
	u32 bind_info_size;

	u32 weak_bind_info_offset;
	u32 weak_bind_info_size;

	u32 lazy_bind_info_offset;
	u32 lazy_bind_info_size;

	u32 export_info_offset;
	u32 export_info_size;
};

UTOPIA_ENUM(u8, relocation_type, ma) {
	macho_relocation_type_pointer = 1,
	macho_relocation_type_text_absolute_32 = 2,
	macho_relocation_type_text_pc_relative_32 = 3,
};

UTOPIA_ENUM(u8, rebase_opcode, ma) {
	macho_rebase_opcode_done = 0,
	macho_rebase_opcode_set_type_immediate = 1,
	macho_rebase_opcode_set_segment_immediate_and_offset_uleb = 2,
	macho_rebase_opcode_add_address_uleb = 3,
	macho_rebase_opcode_add_immediate_scaled = 4,
	macho_rebase_opcode_perform_rebase_immediate_times = 5,
	macho_rebase_opcode_perform_rebase_uleb_times = 6,
	macho_rebase_opcode_perform_rebase_add_uleb = 7,
	macho_rebase_opcode_perform_rebase_uleb_times_skipping_uleb = 8,
};

UTOPIA_ENUM(u8, bind_opcode, ma) {
	macho_bind_opcode_done = 0,
	macho_bind_opcode_set_dylib_ordinal_immediate = 1,
	macho_bind_opcode_set_dylib_ordinal_uleb = 2,
	macho_bind_opcode_set_dylib_special_immediate = 3,
	macho_bind_opcode_set_symbol_trailing_flags = 4,
	macho_bind_opcode_set_type_immediate = 5,
	macho_bind_opcode_set_addend_sleb = 6,
	macho_bind_opcode_set_segment_immediate_and_offset_uleb = 7,
	macho_bind_opcode_add_address_uleb = 8,
	macho_bind_opcode_perform_bind = 9,
	macho_bind_opcode_perform_bind_add_address_uleb = 10,
	macho_bind_opcode_perform_bind_add_address_immediate_scaled = 11,
	macho_bind_opcode_perform_bind_uleb_times_skipping_uleb = 12,
	macho_bind_opcode_threaded = 13,
};

UTOPIA_ENUM(u8, bind_subopcode_threaded, ma) {
	macho_bind_subopcode_threaded_set_bind_ordinal_table_size_uleb = 0,
	macho_bind_subopcode_threaded_apply = 1,
};

UTOPIA_ENUM(u8, export_symbol_kind, ma) {
	macho_export_symbol_kind_regular = 0,
	macho_export_symbol_kind_thread_local = 1,
	macho_export_symbol_kind_absolute = 2,
};

UTOPIA_ENUM(u8, export_symbol_flags, ma) {
	macho_export_symbol_flag_weak = 1 << 0,
	macho_export_symbol_flag_reexport = 1 << 1,
	macho_export_symbol_flag_stub_and_resolver = 1 << 2,
};

UTOPIA_ALWAYS_INLINE u8 macho_relocation_instruction_get_opcode(u8 byte) {
	return byte >> 4;
};

UTOPIA_ALWAYS_INLINE u8 macho_relocation_instruction_get_immediate(u8 byte) {
	return byte & 0x0f;
};

UTOPIA_ALWAYS_INLINE matype_export_symbol_kind macho_export_flags_get_kind(umax raw_flags) {
	return raw_flags & 3ULL;
};

UTOPIA_ALWAYS_INLINE matype_export_symbol_flags macho_export_flags_get(umax raw_flags) {
	return raw_flags >> 2;
};
