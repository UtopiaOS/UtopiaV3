

// Replaced by mdlk on load
.macro stub func
    .text
    .global \func
    \func\():
        ud2
.endm

// TODO: Replace dyld_stub_binder with linker_stub_binder in our patched ld64
stub dyld_stub_binder
stub _mdlk_load_image_by_name
stub _mdlk_load_image_by_name_n
stub _mdlk_load_image_from_file
stub _mdlk_find_loaded_imagine_by_name
stub _mdlk_find_loaded_imagine_by_name_n
stub _mdlk_resolve_symbol
stub _mdlk_resolve_symbol_n