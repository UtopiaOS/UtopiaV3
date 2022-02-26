
#include <covenant/gdat.h>

ctype_hm_bucket c_hm_bucket_init(u64, u64);
ctype_hm_bucket* c_hm_bucket_at(ctype_hmap*, size);
ctype_hmap *c_hm_init(size, size, ctype_hashfn, ctype_cmpfn, void(*obj_free)(void *item), void *);
ctype_status c_hm_insert(ctype_hmap*, void *);