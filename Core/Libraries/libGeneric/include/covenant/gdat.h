
#include <covenant/fn.h>
#define __NEED_u64
#define __NEED_i32
#define __NEED_size
#define __NEED_uchar
#include <covenant/bits/everytype.h>

typedef struct ctype_hmap ctype_hmap;

struct ctype_hmap {
    size obj_size;
    size cap;
    u64 seed_zero;
    u64 seed_one;
    ctype_hashfn hash;
    ctype_cmpfn cmp;
    void (*obj_free)(void *item);
    void *data;
    size bucket_size;
    size nbuckets;
    size count;
    size mask;
    size grow_at;
    size shrink_at;
    void *buckets;
    void *spare;
    void *obj_data;
};

typedef struct ctype_hm_bucket ctype_hm_bucket;

struct ctype_hm_bucket {
    u64 hash;
    u64 dib;
};