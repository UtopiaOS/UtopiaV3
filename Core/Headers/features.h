/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2005-2020 Richard Felker
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * $Utopia$
 */
#ifndef _FEATURES_H
#define _FEATURES_H

#define __COVENANT__ 1

/*
 * Macro to test if we're using a specific version of gcc or later.
 */
#if defined(__GNUC__)
#define	__GNUC_PREREQ__(ma, mi)	\
	(__GNUC__ > (ma) || __GNUC__ == (ma) && __GNUC_MINOR__ >= (mi))
#else
#define	__GNUC_PREREQ__(ma, mi)	0
#endif /* __GNUC_PREREQ__ DEFINITION */


#if defined(_ALL_SOURCES) && !defined(_GNU_SOURCE)
#define _GNU_SOURCE
#endif /* ALL_SOURCES && !GNU_SOURCE */

#if defined(_ALL_SOURCE) && !defined(_GNU_SOURCE)
#define _GNU_SOURCE 1
#endif /* _ALL_SOURCE && !_GNU_SOURCE */

#if defined(_DEFAULT_SOURCE) && !defined(_BSD_SOURCE)
#define _BSD_SOURCE 1
#endif /* DEFAULT_SOURCE && !BSD_SOURCE */

#if !defined(_POSIX_SOURCE) && !defined(_POSIX_C_SOURCE) \
 && !defined(_XOPEN_SOURCE) && !defined(_GNU_SOURCE) \
 && !defined(_BSD_SOURCE) && !defined(__STRICT_ANSI__)
#define _BSD_SOURCE 1
#define _XOPEN_SOURCE 700
#endif /* _POSIX_SOURCE !POSIX_C_SOURCE !_XOPEN_SOURCE !_GNU_SOURCE !_BSD_SOURCE */

#if __STDC_VERSION__ >= 199901L
#define __restrict restrict
#elif !defined(__GNUC__)
#define __restrict
#endif /* restrict */

#if __STDC_VERSION__ >= 199901L || defined(__cplusplus)
#define __inline inline
#elif !defined(__GNUC__)
#define __inline
#endif /* __inline */

#if __STDC_VERSION__ >= 201112L
#elif defined(__GNUC__)
#define _Noreturn __attribute__((__noreturn__))
#else
#define _Noreturn
#endif /* _Noreturn */

#if __GNUC_PREREQ__(2, 96)
#define __malloc_like       __attribute__((__malloc__))
#define __pure              __attribute__((__pure__))
#else
#define __malloc_like
#define __pure
#endif /* __GNUC_PREREQ__ */

#define __REDIR(x,y) __typeof__(x) x __asm__(#y)

#endif /* _FEATURES_H */