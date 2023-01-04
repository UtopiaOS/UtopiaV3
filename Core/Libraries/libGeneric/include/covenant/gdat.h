#ifndef COVENANT_GDAT_H
#define COVENANT_GDAT_H


#include <covenant/fn.h>
#define __NEED_UInt64
#define __NEED_Int32
#define __NEED_Size
#define __NEED_UChar
#include <covenant/bits/everytype.h>

typedef struct HashMap HashMap;

struct HashMap {
    Size obj_size;
    Size cap;
    Int64 seed_zero;
    Int64 seed_one;
    HashFunction hash;
    CompareFunction cmp;
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

typedef struct HashMapBucket HashMapBucket;

struct HashMapBucket {
    Int64 hash;
    Int64 dib;
};

#endif /* COVEANT_GDAT_H */