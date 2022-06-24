
#include <covenant/gdat.h>

void *
c_hm_bucket_item(ctype_hm_bucket *entry)
{
    return ((char *)entry) + sizeof(ctype_hm_bucket);
}