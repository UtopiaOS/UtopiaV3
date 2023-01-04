
#include "hm_impl.h"

Int64 
hm_get_hash(HashMap *hm_map, const void *key)
{
    return hm_map->hash(key, hm_map->seed_zero, hm_map->seed_one) << 16 >> 16;
}