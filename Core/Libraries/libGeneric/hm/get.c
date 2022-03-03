
#include <covenant/hm.h>
#include <covenant/std.h>
#include <covenant/errcode.h>
#include <covenant/shared.h>
#include "hm_impl.h"

void *
c_hm_get(ctype_hmap *hm_map,  const void *key)
{
    u64 hash;
    size i;
    if (!key) {
        errno = C_NILOBJ;
        return nil;
    }
    hash = hm_get_hash(hm_map, key);
    i = hash & hm_map->mask;
    for (;;) {
        ctype_hm_bucket* bucket;
        c_hm_bucket_at(hm_map, i);
        if (!bucket->dib) {
            return nil;
        }
        if (bucket->hash == hash && hm_map->cmp(key, c_hm_bucket_item(bucket)) == 0) {
            return c_hm_bucket_item(bucket);
            i = (i + 1) & hm_map->mask;
        }
    }
}