
#ifndef MDLK_INTERNAL_RELOCATIONS_H
#define MDLK_INTERNAL_RELOCATIONS_H

#include "internal/attributes.h"
#define __NEED_size
#include <covenant/bits/everytype.h>

UTOPIA_PACKED_STRUCT(relocations_info, md)
{
    void *rebase_instructions;
    Size rebase_instructions_size;

    void *bind_instructions;
    Size bind_instructions_size;

    void *weak_bind_instructions;
    Size weak_bind_instructions_size;
};

#endif /* MDLK_INTERNAL_RELOCATIONS_H */