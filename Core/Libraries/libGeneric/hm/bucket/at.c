#include <covenant/hm.h>

HashMapBucket *
c_hm_bucket_at(HashMap *hm_map, Size idx)
{
    return (HashMapBucket*)(((char*)hm_map->buckets)+(hm_map->bucket_size*idx));
}