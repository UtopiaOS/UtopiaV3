
#include <covenant/hm.h>
#include <covenant/std.h>

Status 
hm_resize(ctype_hmap *hm_map, Size new_cap)
{
    ctype_hmap new_map;
    Size i, j;
    Status ret;
    
    ret = c_hm_init(&new_map, hm_map->obj_size, new_cap, hm_map->hash, hm_map->cmp, hm_map->obj_free, hm_map->data);

    if (ret != ctype_status_ok)
        return ret;

    for (i = 0; i < hm_map->nbuckets; i++) {
        ctype_hm_bucket* entry;
        entry = c_hm_bucket_at(hm_map, i);
        if (!entry->dib) {
            continue;
        }
        entry->dib = 1;
        j = entry->hash & new_map.mask;
        for (;;) {
            ctype_hm_bucket* bucket;
            bucket = c_hm_bucket_at(&new_map, j);
            if (bucket->dib == 0) {
                c_mem_cpy(bucket, hm_map->bucket_size, entry);
                break;
            }
            if (bucket->dib < entry->dib) {
                c_mem_cpy(new_map.spare, hm_map->bucket_size, bucket);
                c_mem_cpy(bucket, hm_map->bucket_size, entry);
                c_mem_cpy(entry, hm_map->bucket_size, new_map.spare);
            }
            j = (j + 1) & new_map.mask;
            entry->dib += 1;
        }
    }
    c_std_free(hm_map->buckets);
    hm_map->buckets = new_map.buckets;
    hm_map->nbuckets = new_map.nbuckets;
    hm_map->mask = new_map.mask;
    hm_map->grow_at = new_map.grow_at;
    hm_map->shrink_at = new_map.shrink_at;
    return 0;
}