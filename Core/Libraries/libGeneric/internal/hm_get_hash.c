
#include "hm_impl.h"

u64 
hm_get_hash(ctype_hmap *hm_map, const void *key)
{
    return hm_map->hash(key, hm_map->seed_zero, hm_map->seed_one) << 16 >> 16;
}