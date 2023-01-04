#ifndef COVENANT_GDAT_H
#define COVENANT_GDAT_H


#include <covenant/fn.h>
#define __NEED_u64
#define __NEED_i32
#define __NEED_size
#define __NEED_uchar
#include <covenant/bits/everytype.h>

typedef struct ctype_hmap ctype_hmap;

struct ctype_hmap {
    Size obj_size;
    Size cap;
    Int64 seed_zero;
    Int64 seed_one;
    ctype_hashfn hash;
    ctype_cmpfn cmp;
    void (*obj_free)(void *item);
    void *data;
    Size bucket_size;
    Size nbuckets;
    Size count;
    Size mask;
    Size grow_at;
    Size shrink_at;
    void *buckets;
    void *spare;
    void *obj_data;
};

typedef struct ctype_hm_bucket ctype_hm_bucket;

struct ctype_hm_bucket {
    Int64 hash;
    Int64 dib;
};

#endif /* COVEANT_GDAT_H */