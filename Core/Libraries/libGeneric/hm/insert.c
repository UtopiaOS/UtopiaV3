
#include <covenant/hm.h>
#include <covenant/std.h>
#include <covenant/shared.h>
#include <covenant/errcode.h>
#include "hm_impl.h"

Status
c_hm_insert(HashMap *hm_map, void *obj)
{
  HashMapBucket *entry;
  Size i;

  if (!obj)
  {
    errno = C_NILOBJ;
    return -1;
  }

  if (hm_map->count == hm_map->grow_at)
  {
    if ((hm_resize(hm_map, hm_map->nbuckets * 2)) < 0)
    {
      errno = C_ENOMEM;
      return -1;
    }
  }

  entry = hm_map->obj_data;
  entry->hash = hm_get_hash(hm_map, obj);
  entry->dib = 1;
  c_mem_cpy(c_hm_bucket_item(entry), hm_map->obj_size, obj);

  i = entry->hash & hm_map->mask;
  for (;;)
  {
    HashMapBucket *bucket;
    bucket = c_hm_bucket_at(hm_map, i);
    if (bucket->dib == 0)
    {
      c_mem_cpy(bucket, hm_map->bucket_size, entry);
      hm_map->count++;
      return 0; // First entry in bucket
    }
    if (entry->hash == bucket->hash && (hm_map->cmp(c_hm_bucket_item(entry), c_hm_bucket_item(bucket)) == 0))
    {
      c_mem_cpy(hm_map->spare, hm_map->bucket_size, bucket);
      c_mem_cpy(c_hm_bucket_item(bucket), hm_map->obj_size, c_hm_bucket_item(entry));
      return 0;
    }
    if (bucket->dib < entry->dib)
    {
      c_mem_cpy(hm_map->spare, hm_map->bucket_size, bucket);
      c_mem_cpy(bucket, hm_map->bucket_size, entry);
      c_mem_cpy(entry, hm_map->bucket_size, hm_map->spare);
    }
    i = (i + 1) & hm_map->mask;
    entry->dib += 1;
  }
}