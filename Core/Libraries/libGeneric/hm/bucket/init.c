#include <covenant/hm.h>


ctype_hm_bucket 
c_hm_bucket_init(u64 hash_size, u64 dib_size)
{
    u64 phash_size, pdib_size;
    ctype_hm_bucket pbucket;

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