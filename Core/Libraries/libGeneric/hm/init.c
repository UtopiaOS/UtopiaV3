#include <covenant/hm.h>
#include <covenant/std.h>

// TODO: Implement variadic function for seed
// TODO: We might have to redo some of these parameters...
ctype_status
c_hm_init(ctype_hmap *self, size obj_size, size cap, ctype_hashfn hash, ctype_cmpfn cmp, void (*obj_free)(void *item), void *data)
{
    i32 ncap;
    size bucket_size, hm_size;
    ctype_hm_bucket pbucket;
    ncap = 16;
    if (cap < ncap)
    {
        cap = ncap;
    }
    else
    {
        while (ncap < cap)
        {
            ncap *= 2;
        }
        cap = ncap;
    }

    bucket_size = sizeof(ctype_hm_bucket) + obj_size;
    while (bucket_size & (sizeof(uintptr) - 1))
        bucket_size++;

    
    c_mem_set(self, sizeof(ctype_hmap), 0);

    self->obj_size = obj_size;
    self->bucket_size = bucket_size;
    // TODO: Re-implement this as variadic functions intead of hardcoding!
    self->seed_zero = 0;
    self->seed_one = 0;
    self->hash = hash;
    self->cmp = cmp;
    //! TODO: We might have to make this a type function, unkown if this is usefull
    //! for the rest of libgeneric
    //! NOTICE THAT WE WANT TO AVOID REPEATING CODE
    self->obj_free = obj_free;
    self->data = data;
    self->spare = ((char *)self) + sizeof(ctype_hmap);
    self->obj_data = (char *)self->spare + bucket_size;
    self->cap = cap;
    self->nbuckets = cap;
    self->mask = self->nbuckets - 1;
    self->buckets = c_std_malloc(self->bucket_size * self->nbuckets);
    if (!self->buckets)
        return ctype_status_err;
    c_mem_set(self->buckets, self->bucket_size * self->nbuckets, 0);
    self->grow_at = self->nbuckets * 0.75;
    self->shrink_at = self->nbuckets * 0.10;
    return ctype_status_ok;
}