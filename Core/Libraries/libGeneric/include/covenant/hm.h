
#include <covenant/gdat.h>
#include <covenant/bits/everytype.h>
#include <covenant/status.h>

HashMapBucket c_hm_bucket_init(Int64, Int64);
HashMapBucket* c_hm_bucket_at(HashMap*, Size);
void* c_hm_bucket_item(HashMapBucket*);
void* c_hm_get(HashMap*, void*);
Status c_hm_init(HashMap*, Size, Size, HashFunction, CompareFunction, void(*obj_free)(void *item), void *);
Status c_hm_insert(HashMap*, void *);