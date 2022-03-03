#include <covenant/hm.h>

ctype_hm_bucket *
c_hm_bucket_at(ctype_hmap *hm_map, size idx)
{
    return (ctype_hm_bucket*)(((char*)hm_map->buckets)+(hm_map->bucket_size*idx));
}