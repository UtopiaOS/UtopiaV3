
#include <covenant/gdat.h>
#include <covenant/bits/everytype.h>
#include <covenant/status.h>

ctype_hm_bucket c_hm_bucket_init(u64, u64);
ctype_hm_bucket* c_hm_bucket_at(ctype_hmap*, size);
void* c_hm_bucket_item(ctype_hm_bucket*);
void* c_hm_get(ctype_hmap*, void*);
ctype_status c_hm_init(ctype_hmap*, size, size, ctype_hashfn, ctype_cmpfn, void(*obj_free)(void *item), void *);
ctype_status c_hm_insert(ctype_hmap*, void *);