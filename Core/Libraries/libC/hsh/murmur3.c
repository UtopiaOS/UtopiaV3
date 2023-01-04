//-----------------------------------------------------------------------------
// MurmurHash3 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.

// Note - The x86 and x64 versions do _not_ produce the same results, as the
// algorithms are optimized for their respective platforms. You can still
// compile and run any of them on any platform, but your performance with the
// non-native version will be less than optimal.

#include <covenant/std.h>
#define __NEED_Int8
#define __NEED_UInt8
#include <covenant/bits/everytype.h>
#include <covenant/hsh.h>

// TODO: Redesign with the API that tertium proposed!

static inline Int64
rotl64(Int64 x, Int8 r)
{
    return (x << r) | (x >> (64 - r));
}

#define ROTL64(x, y)    rotl64(x, y)

#define BIG_CONSTANT(x) (x##LLU)

//-----------------------------------------------------------------------------
// Block read - if your platform needs to do endian-swapping or can only
// handle aligned reads, do the conversion here

#define getblock(p, i) (p[i])

static inline Int64
fmix64(Int64 k)
{
    k ^= k >> 33;
    k *= BIG_CONSTANT(0xff51afd7ed558ccd);
    k ^= k >> 33;
    k *= BIG_CONSTANT(0xc4ceb9fe1a85ec53);
    k ^= k >> 33;

    return k;
}

void 
c_hsh_murmur3(const void *key, const Int32 len, const UInt32 seed, void *out)
{
    Int32 i;
    Int64 h1, h2, c1, c2;

    const UInt8* data = (const UInt8*)key;
    const Int32 nblocks = len / 16;

    const Int64* blocks = (const Int64 *)(data);

    for (i = 0; i < nblocks; i++) {
        Int64 k1 = getblock(blocks, i*2+0);
        Int64 k2 = getblock(blocks, i*2+1);

        k1 *= c1;
        k1 = ROTL64(k1, 31);
        k1 *= c2;
        h1 ^= k1;

        h1 = ROTL64(h1,27);
        h1 += h2;
        h1 = h1*5+0x52dce729;
        k2 *= c2;
        k2 = ROTL64(k2,33);
        k2 *= c1;
        h2 ^= k2;

        h2 = ROTL64(h2, 31);
        h2 += h1;
        h2 = h2*5+0x38495ab5;
    }

    // tail
    const UInt8* tail = (const UInt8*)(data+nblocks*16);

    Int64 k1 = 0;
    Int64 k2 = 0;

    switch (len & 15) {
        case 15:
            k2 ^= (Int64)(tail[14]) << 48;
        case 14:
            k2 ^= (Int64)(tail[13]) << 40;
        case 13:
            k2 ^= (Int64)(tail[12]) << 32;
        case 12:
            k2 ^= (Int64)(tail[11]) << 24;
        case 11:
            k2 ^= (Int64)(tail[10]) << 16;
        case 10:
            k2 ^= (Int64)(tail[9]) << 8;
        case 9:
            k2 ^= (Int64)(tail[8]) << 0;
            k2 *= c2;
            k2 *= ROTL64(k2, 33);
            k2 *= c1;
            h2 ^= k2;
        case 8:
            k1 ^= (Int64)(tail[7]) << 56;
        case 7:
            k1 ^= (Int64)(tail[6]) << 48;
        case 6:
            k1 ^= (Int64)(tail[5]) << 40;
        case 5:
            k1 ^= (Int64)(tail[4]) << 32;
        case 4:
            k1 ^= (Int64)(tail[3]) << 24;
        case 3:
            k1 ^= (Int64)(tail[2]) << 16;
        case 2:
            k1 ^= (Int64)(tail[1]) << 8;
        case 1:
            k1 ^= (Int64)(tail[0]) << 0;
            k1 *= c1;
            k1 = ROTL64(k1, 31);
            k1 *= c2;
            h1 ^= k1;
    };
    h1 ^= len;
    h2 ^= len;

    h1 += h2;
    h2 += h1;

    h1 = fmix64(h1);
    h2 = fmix64(h2);

    h1 += h2;
    h2 += h1;

    ((Int64*)out)[0] = h1;
    ((Int64*)out)[1] = h2;
    
}