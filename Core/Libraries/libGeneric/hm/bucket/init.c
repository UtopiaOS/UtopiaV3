#include <covenant/hm.h>


HashMapBucket 
c_hm_bucket_init(Int64 hash_size, Int64 dib_size)
{
    Int64 phash_size, pdib_size;
    HashMapBucket pbucket;

    phash_size = 48;
    pdib_size = 16;

    if (hash_size != 0){
        phash_size = hash_size;
    }

    if (dib_size != 0) {
        pdib_size = dib_size;
    }

    pbucket.hash = phash_size;
    pbucket.dib = pdib_size;

    return pbucket;
}