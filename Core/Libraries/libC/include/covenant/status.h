
/**
 * @file
 * 
 * Utopia error routines.
 */

#ifndef _COVENANT_ERROR_H_
#define _COVENANT_ERROR_H_

#define __NEED_i32
#define __NEED_Int32
#include <covenant/bits/everytype.h>
#include "internal/attributes.h"

/**
 * @addtogroup Status
 * 
 * Utopia Covenant libc error codes.
 * 
 * Unlike POSIX error codes, these ones
 * start with negative numbers. This is to avoid 
 * conflict.
 *
 */
UTOPIA_NEO_ENUM(Int32, Status) {
    /**
     * No error
     * 
     */
    StatusOk = 0,

    /**
     * Generic error
     * 
     */
    StatusErr = -1,
};

#endif