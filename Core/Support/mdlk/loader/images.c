#include <covenant/std.h>
#include <covenant/hsh.h>
#include <covenant/limits.h>
#include <covenant/hm.h>
#include <libmacho/libmacho.h>
#include <covenant/bool.h>
#include "internal/_images.h"
#include "internal/_relocations.h"

const char EXECUTABLE_PATH[]="executable_path=";
#define LIBMDLK_PATH "/Core/Support/libmdlk.dylib"

static ctype_hmap images;

Int64 mdlk_basic_image_hash(const void *key, Int64 seed_one, Int64 seed_two)
{
    const mdtype_mdlk_image *image = key;
    ctype_hst hs;
    ctype_hmd *md;
    char out[16];
    md = c_hsh_murmur32;
    md->init(&hs);
    md->update(&hs, image->path, c_str_len(image->path, C_USIZE_MAX));
    md->end(&hs, out);
    return *(Int64 *)out;
}

Int32 mdlk_basic_images_cmp(void *a, void *b)
{
    const mdtype_mdlk_image* image_a = a;
    const mdtype_mdlk_image* image_b = b;
    // TODO: Make this PATH_MAX
    return c_str_cmp(image_a->path, C_USIZE_MAX, image_b->path);
}

char* mdlk_get_executable_path(const char** utopia_pointers)
{
    return utopia_pointers[0] + sizeof(EXECUTABLE_PATH) - 1;
}

static ctype_status mdlk_load_image_internal(const char* path, size path_len, mdtype_mdlk_image** out_image) {
    ctype_status status = ctype_status_ok;
    matype_macho_header header = {0};
    mdtype_mdlk_image* image = nil;
    ctype_file *file = nil;
    bool created = false;
    void* file_load_top;

    matype_load_command load_command = {0};
    size file_offset = 0;
    
    size entry_point_file_offset = C_SIZE_MAX;
    mdtype_relocations_info relocation_info = {0};

    image = c_hm_get(&images, &(mdtype_mdlk_image){.path = path});

    // Create image
    if (!image) {
        image = c_std_malloc(sizeof(mdtype_mdlk_image));
        if (!image) {
            status = ctype_status_err;
            goto out;
        }
        // TODO: If this is always necessary, is better to create a PATH_MAX constant
        image->path = c_std_malloc(sizeof(char) * 4096);
        c_str_cpy(image->path, C_USIZE_MAX, path);
        status = c_hm_insert(&images, image);
        if (status != ctype_status_ok) {
            status = ctype_status_err;
            goto out;
        }
        created = true;
    }

    // We already had the image.
    // Since we first search using the hm_get function, and we set
    // the created variable to "true" only if the "image" wasn't
    // already in the hashmap, we can asume that if we got here, it meant
    // we already had the image loaded!
    if (!created) {
        // Sitently ignore the fact we have created it already, just return ok
        status = ctype_status_ok;
        goto out;
    }

    c_mem_set(image, sizeof(*image), 0);
    
    image->file_load_base = (void*)C_UINTPTR_MAX;
    image->is_libmdlk = ((path_len == sizeof(LIBMDLK_PATH) - 1) && (c_str_cmp(path, C_USIZE_MAX, LIBMDLK_PATH) == 0));

    // TODO: See how are we gonna fill the name, for now, lets handle it like this:
    if (!image->path)
        image->path = c_std_malloc(sizeof(char) * 4096);

    c_str_cpy(image->path, C_USIZE_MAX, path);
    image->path_lenght = c_str_len(image->path, C_USIZE_MAX);

    // We can now read into the header

    file = c_file_open(path, "rb");
    if (!file) {
        status = ctype_status_err;
        goto out;
    }

    c_file_feed(file);

    c_file_get(file, &header, sizeof(matype_macho_header));
    file_offset = sizeof(header);
    c_file_seek(file, file_offset);

    for (size i = 0; i < header.command_count; (++i), (file_offset += load_command.size)) {
        c_file_seek(file, file_offset);
        matype_load_command_segment_64 segment_64_load_command = {0};

        c_file_get(file, &load_command, sizeof(load_command));
        if (load_command.type != macho_load_command_type_segment_64) {
            continue;
        }

        c_file_get(file, &segment_64_load_command, sizeof(segment_64_load_command));

        image->section_count += segment_64_load_command.section_count;
        ++image->section_count;

        if (segment_64_load_command.initial_memory_protection == 0 && segment_64_load_command.maximum_memory_protection == 0) {
            continue;
        }

        if (segment_64_load_command.memory_address < (UIntPtr)image->file_load_base) {
            image->file_load_base = (void*)segment_64_load_command.memory_address;
        }

        if (segment_64_load_command.memory_address + segment_64_load_command.memory_size > (UIntPtr)file_load_top) {
            file_load_top = (void*)(segment_64_load_command.memory_address + segment_64_load_command.memory_size);
        } 
    }

    image->total_size = (Int64)(file_load_top - image->file_load_base);


    return ctype_status_ok;
out:
    return status;
}


ctype_status mdlk_images_init(mdtype_mdlk_image **out_image, char* path)
{
    ctype_status ret;

    ret = c_hm_init(&images, sizeof(mdtype_mdlk_image), 4, mdlk_basic_image_hash, mdlk_basic_images_cmp, nil, nil);

    if (ret != ctype_status_ok) {
        c_ioq_fmt(ioq2, "Error!\n");
    }

    mdlk_load_image_internal(path, c_str_len(path, C_USIZE_MAX), out_image);

    return ctype_status_ok;
}