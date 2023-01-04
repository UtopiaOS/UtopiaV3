
#include <covenant/gdat.h>
#include <covenant/bits/everytype.h>
#include <covenant/status.h>

ctype_hm_bucket c_hm_bucket_init(Int64, Int64);
ctype_hm_bucket* c_hm_bucket_at(ctype_hmap*, Size);
void* c_hm_bucket_item(ctype_hm_bucket*);
void* c_hm_get(ctype_hmap*, void*);
Status c_hm_init(ctype_hmap*, Size, Size, ctype_hashfn, ctype_cmpfn, void(*obj_free)(void *item), void *);
Status c_hm_insert(ctype_hmap*, void *);