#include <covenant/hm.h>
#include <covenant/std.h>

// TODO: Implement variadic function for seed
// TODO: We might have to redo some of these parameters...
ctype_hmap *
c_hm_init(size obj_size, size cap, ctype_hashfn hash, ctype_cmpfn cmp, void(*obj_free)(void *item), void *data) 
{
    i32 ncap;
    size bucket_size, hm_size;
    ctype_hm_bucket pbucket;
    ctype_hmap *hm_map;
    ncap = 16;
    if (cap < ncap) {
        cap = ncap;
    } else {
        while (ncap < cap) {
            ncap *= 2;
        }
        cap = ncap;
    }

    // TODO: is this ok?, should we initialize like this?
    pbucket = c_hm_bucket_init(0, 0);
    bucket_size = sizeof(pbucket) + obj_size;

    hm_size = sizeof(ctype_hmap)+bucket_size*2; 
    
    /* TODO: We might be able to allocate proper memory blocks here
       because of the way our allocator is implemented... */
    if (!(c_std_alloc(hm_size, sizeof(uchar))))
        return nil;

    c_mem_set(hm_map, 0, sizeof(ctype_hmap));

    hm_map->obj_size = obj_size;
    hm_map->bucket_size = bucket_size;
    // TODO: Re-implement this as variadic functions intead of hardcoding!
    hm_map->seed_zero = 0;
    hm_map->seed_one = 0;
    hm_map->hash = hash;
    hm_map->cmp = cmp;
    //! TODO: We might have to make this a type function, unkown if this is usefull
    //! for the rest of libgeneric
    //! NOTICE THAT WE WANT TO AVOID REPEATING CODE
    hm_map->obj_free = obj_free;
    hm_map->data = data;
    hm_map->spare = ((char*)hm_map)+sizeof(ctype_hmap);
    hm_map->obj_data = (char*)hm_map->spare+bucket_size;
    hm_map->cap = cap;
    hm_map->nbuckets = cap;
    hm_map->mask = hm_map->nbuckets - 1;
    hm_map->buckets = c_std_alloc(hm_map->bucket_size * hm_map->nbuckets, sizeof(uchar));
    if (!hm_map->buckets) {
        c_std_free(hm_map);
        return nil;
    }
    c_mem_set(hm_map->buckets, 0, hm_map->bucket_size * hm_map->nbuckets);
    hm_map->grow_at = hm_map->nbuckets * 0.75;
    hm_map->shrink_at = hm_map->nbuckets * 0.10;
    return hm_map;
}