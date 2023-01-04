#include <covenant/std.h>
#include <covenant/hsh.h>

#define C1 0xCC9E2D51
#define C2 0x1B873593

static void init(HST *);
static void update(HST *, char *, USize);
static void end(HST *, char *);

static HMD md = {
    &init,
    &update,
    &end,
};

HMD *c_hsh_murmur32 = &md;

static void
init(HST *p)
{
    p->curlen = 0;
    p->len = 0;
    p->st.x32[0] = 0xDEAFB0FAU;
}

static void
compress(HST *p, char *data)
{
    UInt32 k;
    k = c_u32_unpack(data);
    k *= C1;
    k = c_hsh_rol32(k, 15);
    k *= C2;
    p->st.x32[0] ^= k;
    p->st.x32[0] = c_hsh_rol32(p->st.x32[0], 13);
    p->st.x32[0] = p->st.x32[0] * 5 + 0xE6546B64;
}

static void
update(HST *p, char *data, USize n)
{
    c_hsh_update(compress, 4, p, data, n);
}

static void
end(HST *p, char *s)
{
    UInt32 k;

    k = 0;
    switch (p->curlen)
    {
    case 3:
        k ^= p->buf[2] << 16;
        /* FALLTHROUGH */
    case 2:
        k ^= p->buf[1] << 8;
        /* FALLTHROUGH */
    case 1:
        k ^= p->buf[0];
        k *= C1;
        k = c_hsh_rol32(k, 15);
        k *= C2;
    }
    p->st.x32[0] ^= k;
    p->len += p->curlen << 3;
    p->st.x32[0] ^= c_hsh_octets(p);
    /* mix */
    p->st.x32[0] ^= p->st.x32[0] >> 16;
    p->st.x32[0] *= 0x85EBCA6B;
    p->st.x32[0] ^= p->st.x32[0] >> 13;
    p->st.x32[0] *= 0xC2B2AE35;
    p->st.x32[0] ^= p->st.x32[0] >> 16;
    /* digest */
    c_u32_pack(s, p->st.x32[0]);
}