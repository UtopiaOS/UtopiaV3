
#include <covenant/std.h>

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
    size bucketsz;
    size nbuckets;
    size count;
    size mask;
    size grow_at;
    size shrink_at;
    void *buckets;
    void *spare;
    void *obj_data;
};