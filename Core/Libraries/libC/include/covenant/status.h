
/**
 * @file
 * 
 * Utopia error routines.
 */

#ifndef _COVENANT_ERROR_H_
#define _COVENANT_ERROR_H_

#define __NEED_i32
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

UTOPIA_ENUM(i32, status, c) {
    /**
     * No error
     * 
     */
    ctype_status_ok = 0,

    /**
     * Generic error
     * 
     */
    ctype_status_err = -1,
};

#endif