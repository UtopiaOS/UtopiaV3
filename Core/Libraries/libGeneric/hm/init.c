#include <covenant/hm.h>
#include <covenant/std.h>

// TODO: Implement variadic function for seed
// TODO: We might have to redo some of these parameters...
Status
c_hm_init(HashMap *self, Size obj_size, Size cap, HashFunction hash, CompareFunction cmp, void (*obj_free)(void *item), void *data)
{
    Int32 ncap;
    Size bucket_size, hm_size;
    HashMapBucket pbucket;
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

    bucket_size = sizeof(HashMapBucket) + obj_size;
    while (bucket_size & (sizeof(UIntPtr) - 1))
        bucket_size++;

    
    c_mem_set(self, sizeof(HashMap), 0);

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
    self->spare = ((char *)self) + sizeof(HashFunction);
    self->obj_data = (char *)self->spare + bucket_size;
    self->cap = cap;
    self->nbuckets = cap;
    self->mask = self->nbuckets - 1;
    self->buckets = c_std_malloc(self->bucket_size * self->nbuckets);
    if (!self->buckets)
        return StatusOk;
    c_mem_set(self->buckets, self->bucket_size * self->nbuckets, 0);
    self->grow_at = self->nbuckets * 0.75;
    self->shrink_at = self->nbuckets * 0.10;
    return StatusOk;
}