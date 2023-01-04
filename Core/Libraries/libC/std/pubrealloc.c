/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <phk@FreeBSD.ORG> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Poul-Henning Kamp
 * ----------------------------------------------------------------------------
 */

#include <covenant/std.h>
#include <covenant/sys/mman.h>
#include <covenant/kernel/basic.h>
#include "std_impl.h"
#include <covenant/sys/syscall.h>
#include "syscall.h"
#include <covenant/shared.h>
#include <covenant/errcode.h>
#include <covenant/nix.h>
#include <covenant/limits.h>

#define EMSG(a) c_kernel_fdwrite(2, (a), sizeof((a)))

//Int64 __mmap_base = 0x7fff00000000ULL;

Int64 be_random_number = 20;

#define MMAP(a) \
    c_kernel_mmap(0, (a), C_PROT_READ | C_PROT_WRITE, C_MAP_ANON | C_MAP_PRIVATE, -1, 0)

#define MNOTMINE ((struct pginfo *)0)
#define MFREE ((struct pginfo *)1)
#define MFIRST ((struct pginfo *)2)
#define MFOLLOW ((struct pginfo *)3)
#define MMAGIC ((struct pginfo *)4)

#define MBITS ((Int32)(8 * sizeof(UInt32)))

#define mminsize 16U
#define mmaxsize ((mpagesize) >> 1)

#define pageround(a) (((a) + mpagemask) & (~mpagemask))
#define ptr2idx(a) (((USize)(UIntPtr)(a) >> mpageshift) - morigo)

struct pginfo
{
    struct pginfo *next;
    void *page;
    UShort size;
    UShort shift;
    UShort free;
    UShort total;
    UInt32 bits[1];
};

struct pgfree
{
    struct pgfree *next;
    struct pgfree *prev;
    void *page;
    void *end;
    USize size;
};

static union
{
    UChar uc[16];
    long double ld;
} const zeroblock;

char *alloc0 = (char *)(UIntPtr)&zeroblock;

static struct pginfo **pagedir;
static struct pgfree freelist;
static struct pgfree *px; /* Freelist cache */
static USize lastidx;     /* Last relevant index in page dir */
static USize mcache = 16; /* freepages to cache */
static USize minfo;
static USize morigo;
static USize mpagemask;
static USize mpageshift;
static USize mpagesize;
static Int32 mstt;
static void *cbrk;
static void *mbrk;

static void *imalloc(USize);
static void ifree(void *);

static Int32
_brk(void *p)
{
    cbrk = (void *)(UIntPtr)__syscall(SYS_brk, p);
    return ((cbrk == (void *)-1) ? -1 : 0);
}

static void *
segbrk(UIntPtr p)
{
    void *obj;

    if (!cbrk && _brk(0) < 0)
        return (void *)-1;
    if (!p)
        return cbrk;
    obj = cbrk;
    if (_brk((char *)obj + p) < 0)
        return (void *)-1;
    return obj;
}

static void
freepages(void *p, USize idx, struct pginfo *info)
{
    struct pgfree *pf, *pt;
    USize l, i;
    void *t;

    if (!(info == MFIRST))
        return;

    if ((UIntPtr)p & mpagemask)
        return;

    pagedir[idx] = MFREE;
    for (i = 1; pagedir[idx + i] == MFOLLOW; i++)
    {
        pagedir[idx + i] = MFREE;
    }
    l = i << mpageshift;

    if (!px)
        px = imalloc(sizeof(*px));
    px->page = p;
    px->end = t = (void *)((UChar *)p + l);
    px->size = l;
    pt = nil;
    if (!freelist.next)
    {
        px->next = freelist.next;
        px->prev = &freelist;
        freelist.next = px;
        pf = px;
        px = nil;
    }
    else
    {
        for (pf = freelist.next; pf->end < p && pf->next; pf = pf->next)
            ;
        if (pf->page > t)
        {
            px->next = pf;
            px->prev = pf->prev;
            pf->prev = px->prev->next = px;
            pf = px;
            px = nil;
        }
        else if (pf->end == p)
        {
            pf->end = (UChar *)pf->end + l;
            pf->size += l;
            if (pf->next && pf->end == pf->next->page)
            {
                pt = pf->next;
                pf->end = pt->end;
                pf->size += pt->size;
                if ((pf->next = pt->next))
                    pf->next->prev = pf;
            }
        }
        else if (pf->page == t)
        {
            pf->size += l;
            pf->page = p;
        }
        else if (!pf->next)
        {
            px->next = nil;
            px->prev = pf;
            pf->next = px;
            pf = px;
            px = nil;
        }
        else
        {
            EMSG("freelist is destroyed!\n");
            c_kernel_abort();
        }
    }
    if (!pf->next && pf->size > mcache && pf->end == mbrk && mbrk == segbrk(0))
    {
        pf->end = (UChar *)pf->page + mcache;
        pf->size = mcache;
        _brk(pf->end);
        mbrk = pf->end;
        idx = ptr2idx(pf->end);
        for (i = idx; i <= lastidx; i++)
        {
            pagedir[i] = MNOTMINE;
            lastidx = idx - 1;
        }
    }
    if (pt)
        ifree(pt);
}

static void
freebytes(void *p, USize idx, struct pginfo *info)
{
    struct pginfo **mp;
    USize i;

    /* Modified chunk */
    if ((USize)(UIntPtr)p & (info->size - 1))
        return;

    i = ((USize)(UIntPtr)p & mpagemask) >> info->shift;
    // Already free, ignore it
    if (info->bits[i / MBITS] & (1UL << (i % MBITS)))
        return;

    info->bits[i / MBITS] |= (1UL << (i % MBITS));
    ++info->free;
    mp = pagedir + info->shift;
    if (info->free == 1)
    {
        while (*mp && (*mp)->next && (*mp)->next->page < info->page)
            mp = &(*mp)->next;
        info->next = *mp;
        *mp = info;
        return;
    }
    else if (info->free != info->total)
    {
        return;
    }
    for (; *mp != info; mp = &(*mp)->next)
        ;
    *mp = info->next;
    pagedir[idx] = MFIRST;

    if (info->page != info)
        ifree(info);
    ifree(info->page);
}

static void
ifree(void *p)
{
    struct pginfo *i;
    USize idx;

    if (!p)
        return;

    idx = ptr2idx(p);
    /* Too low or too high */
    if (idx < mpageshift || idx > lastidx)
        return;

    // what?
    if ((i = pagedir[idx]) > MMAGIC)
        freebytes(p, idx, i);
    else
        freepages(p, idx, i);
}

static Int32
extendpgdir(USize idx)
{
    struct pginfo **p, **o;
    USize nl, ol;

    if ((((~(1UL << ((sizeof(USize) * 8) - 1)) / sizeof(*pagedir)) + 1) + (mpagesize / sizeof(*pagedir))) < idx)
    {
        errno = C_ENOMEM;
        return 0;
    }

    nl = pageround(idx * sizeof(*pagedir)) + mpagesize;
    if ((p = MMAP(nl)) == C_MAP_FAILED)
        return 0;

    ol = minfo * sizeof(*pagedir);
    c_mem_cpy(p, ol, pagedir);

    minfo = nl / sizeof(*pagedir);
    o = pagedir;
    pagedir = p;
    c_nix_munmap(o, ol);
    return 1;
}

static void *
mappages(USize pages)
{
    IntPtr bytes;
    void *r, *rr, *t;

    bytes = pages << mpageshift;
    if (bytes < 0 || bytes < (IntPtr)pages)
    {
        errno = C_ENOMEM;
        return nil;
    }

    if ((r = segbrk(bytes)) == (void *)-1)
        return nil;
    if ((rr = (void *)pageround((USize)(UIntPtr)r)) > r)
    {
        if (segbrk((UChar *)rr - (UChar *)r) == (void *)-1 && _brk(r))
        {
            EMSG("brk(2) failed [internal error]\n");
            c_kernel_abort();
        }
    }

    t = (UChar *)rr + bytes;
    lastidx = ptr2idx(t) - 1;
    mbrk = t;
    if ((lastidx + 1) >= minfo && !extendpgdir(lastidx))
    {
        lastidx = ptr2idx((mbrk = r)) - 1;
        if (_brk(mbrk))
        {
            EMSG("brk(2) failed [internal error]\n");
            c_kernel_abort();
        }
        return nil;
    }
    return rr;
}

static void *
allocpages(USize n)
{
    struct pgfree *pf;
    USize i, idx;
    void *p, *df;

    if ((idx = pageround(n)) < n)
    {
        errno = C_ENOMEM;
        return nil;
    }

    n = idx;
    df = p = nil;

    for (pf = freelist.next; pf; pf = pf->next)
    {
        if (pf->size < n)
            continue;
        if (pf->size == n)
        {
            p = pf->page;
            if (pf->next)
                pf->next->prev = pf->prev;
            pf->prev->next = pf->next;
            df = pf;
            break;
        }
        p = pf->page;
        pf->page = (UChar *)pf->page + n;
        pf->size -= n;
        break;
    }
    n >>= mpageshift;
    if (!p)
        p = mappages(n);
    if (p)
    {
        idx = ptr2idx(p);
        pagedir[idx] = MFIRST;
        for (i = 1; i < n; i++)
        {
            pagedir[idx + i] = MFOLLOW;
        }
    }
    if (df)
    {
        if (!px)
            px = df;
        else
            ifree(df);
    }
    return p;
}

static Int32
allocchunks(Int32 bits)
{
    struct pginfo *bp;
    long l;
    Int32 i, k;
    void *pp;

    if (!(pp = allocpages(mpagesize)))
        return 0;

    l = c_std_offsetof(struct pginfo, bits[0]);
    l += sizeof(bp->bits[0]) * (((mpagesize >> bits) + MBITS - 1) / MBITS);
    if ((1 << bits) <= (l << 1))
    {
        bp = pp;
    }
    else
    {
        if (!(bp = imalloc(l)))
        {
            ifree(pp);
            return 0;
        }
    }

    k = bp->total = bp->free = mpagesize >> bits;
    for (i = 0; k - i >= MBITS; i += MBITS)
    {
        bp->bits[i / MBITS] = ~0U;
    }

    for (; i < k; i++)
        bp->bits[i / MBITS] |= 1 << (i % MBITS);

    bp->page = pp;
    if (bp == bp->page)
    {
        for (i = 0; l > 0; i++)
        {
            bp->bits[i / MBITS] &= ~(1 << (i % MBITS));
            --bp->free;
            --bp->total;
            l -= 1 << bits;
        }
    }

    bp->size = (1 << bits);
    bp->shift = bits;
    pagedir[ptr2idx(pp)] = bp;
    bp->next = pagedir[bits];
    pagedir[bits] = bp;
    return 1;
}

static void *
allocbytes(USize n)
{
    struct pginfo *bp;
    USize i, k;
    Int32 j;
    UInt32 *lp;
    UInt32 u;

    if (n < mminsize)
        n = mminsize;

    for (i = n - (j = 1); i >>= 1; j++)
        ;

    if (!pagedir[j] && !allocchunks(j))
        return nil;

    bp = pagedir[j];
    for (lp = bp->bits; !*lp; lp++)
        ;

    u = 1;
    for (k = 0; !(*lp & u); k++)
        u += u;

    if (!--bp->free)
    {
        pagedir[j] = bp->next;
        bp->next = nil;
    }

    *lp ^= u;
    k += (lp - bp->bits) * MBITS;
    k <<= bp->shift;
    return (UChar *)bp->page + k;
}

static void *
imalloc(USize n)
{
    if ((n + mpagesize) < n || (n + mpagesize) >= (UIntPtr)pagedir)
        return nil;
    if (n <= mmaxsize)
        return allocbytes(n);
    return allocpages(n);
}

static void *
irealloc(void *p, USize n)
{
    struct pginfo **mp;
    USize i, o, idx;
    void *np;

    idx = ptr2idx(p);
    /* Too low or too high */
    if (idx < mpageshift || idx > lastidx)
        return nil;

    if (*(mp = &pagedir[idx]) == MFIRST)
    {
        /* modified (page-) pointer */
        if ((USize)(UIntPtr)p & mpagemask)
            return nil;
        for (o = mpagesize; *mp++ == MFOLLOW; o += mpagesize)
            ;
        if (n <= o && n > (o - mpagesize))
            return p;
    }
    else if (*mp >= MMAGIC)
    {
        /* modified (chunk-) pointer */
        if ((USize)(UIntPtr)p & ((*mp)->size - 1))
            return nil;

        i = ((USize)(UIntPtr)p & mpagemask) >> (*mp)->shift;
        /* Chunk is already free */
        if ((*mp)->bits[i / MBITS] & (1UL << (i % MBITS)))
            return nil;

        o = (*mp)->size;
        if (n <= o && (n > (o >> 1) || o == mminsize))
            return p;
    }
    else
    {
        /* Pointer to wrong place */
        return nil;
    }

    if ((np = imalloc(n)))
    {
        if (n && o)
            c_mem_cpy(np, C_MIN(n, o), p);
        ifree(p);
    }
    return np;
}

static void
minit(void)
{
    mpagesize = C_PAGESIZE;
    for (mpageshift = 0; (1UL << mpageshift) != mpagesize; mpageshift++)
        ;
    mpagemask = mpagesize - 1;
    if ((pagedir = MMAP(mpagesize)) == C_MAP_FAILED)
    {
        EMSG("mmap(2) failed, check limits\n");
        c_kernel_abort();
    }
    minfo = mpagesize / sizeof(*pagedir);
    mcache <<= mpageshift;
    morigo = pageround((USize)(UIntPtr)segbrk(0)) >> mpageshift;
    morigo -= mpageshift;
    px = imalloc(sizeof(*px));
}

void *
pubrealloc(void *p, USize m, USize n)
{
    void *r;    
    if (C_OFLW_UM(USize, m, n))
    {
        errno = C_EOVERFLOW;
        return nil;
    }

    r = nil;
    if (!mstt)
    {
        if (p)
            goto invalid;
        minit();
        mstt++;
    }

    if (p == alloc0) {
        goto invalid;
    }

    if (!m)
    {
        if (p)
        {
            ifree(p);
            return nil;
        }
        r = alloc0;
    }
    else
    {
        m *= n;
        if (!(r = p ? irealloc(p, m) : imalloc(m)))
            errno = C_ENOMEM;
    }

    return r;
invalid:
    errno = C_EINVAL;
    return r;
}