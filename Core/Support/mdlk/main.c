

#include <covenant/std.h>
#include <covenant/hsh.h>
#include <covenant/hm.h>
#include <covenant/limits.h>

void start(void) __asm__("start");

struct user {
    char *name;
    char initial;
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
    
    //c_ioq_fmt(ioq2, "Hello world! Again?\n");

    struct user *user = c_std_alloc((sizeof(struct user*)), sizeof(uchar));
    unsigned long long random;

    user->name = c_std_alloc((20 * sizeof(char)), sizeof(uchar));

    c_str_ncpy(user->name, "Diego", C_USIZE_MAX);
    user->age = -18;
    user->initial = 'd';


    c_ioq_fmt(ioq1, "My initial is: %c and my age is: %d and my name is %s and my address in memory is %p\n", user->initial, user->age, user->name, &user);
    c_std_exit(0);
}