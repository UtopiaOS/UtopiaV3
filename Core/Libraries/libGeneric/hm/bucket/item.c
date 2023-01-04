
#include <covenant/gdat.h>

void *
c_hm_bucket_item(HashMapBucket *entry)
{
    return ((char *)entry) + sizeof(HashMapBucket);
}