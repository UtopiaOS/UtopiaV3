

#include <covenant/std.h>
#include <covenant/hsh.h>
#include <covenant/hm.h>
#include <covenant/limits.h>

void start(void) __asm__("start");

struct user {
    char *name;
    i32 age;
};

u64
user_hash(const void *item, u64 seed_zero, u64 seed_one)
{
    const struct user *user = item;
    char out[16];
    c_hsh_murmur3(user->name, c_str_len(user->name, C_USIZE_MAX), 0, &out);
    return *(u64*)out;
}

i32
user_cmp(const void *a, const void *b)
{
    const struct user *ua = a;
    const struct user *ub = b;
    return c_str_cmp(ua->name, C_USIZE_MAX, ub->name);
}


int main() {
    
}

void start(void) {
    u64 hash;
    
    c_ioq_fmt(ioq2, "Hello world! Again?\n");

    struct user *user = c_std_alloc((sizeof(struct user*)), sizeof(uchar));

    user->age = 29;

    c_ioq_fmt(ioq2, "%d\n", user->age);

    c_ioq_fmt(ioq2, "Hello world! Again?\n");
    c_ioq_fmt(ioq2, "%ud", hash);
    ctype_hmap *hm_map = c_hm_init(sizeof(struct user), 0, user_hash, user_cmp, nil, nil);
    c_ioq_fmt(ioq2, "Hello!!\n");
    c_hm_insert(hm_map, &(struct user){ .name = "Diego", .age = 18});
    c_hm_insert(hm_map, &(struct user){ .name = "Clyde", .age = 17});
    c_ioq_fmt(ioq2, "Hello world!\n");
    c_std_exit(0);
}