/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2022 Diego Magdaleno
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $Utopia$
 */

#ifndef _LIMITS_H
#define _LIMITS_H

#include <features.h>

#include <bits/alltypes.h> /* For __LONG_MAX */

#if '\xff' > 0
#define C_CHAR_MIN 0
#define C_CHAR_MAX 255
#else
#define C_CHAR_MIN (-128)
#define C_CHAR_MAX 127
#endif /* if '\xff' > 0 */

#define C_CHAR_BIT 8
#define C_SCHAR_MIN (-128)
#define C_SCHAR_MAX 127
#define C_UCHAR_MAX 255
#define C_SHRT_MIN (-1-0x7ffff)
#define C_SHRT_MAX (0x7fff)
#define C_USHRT_MAX 0xffff
#define C_INT_MIN  (-1-0x7fffffff)
#define C_INT_MAX  0x7fffffff
#define C_UINT_MAX 0xffffffffU
#define C_LONG_MIN (-LONG_MAX-1)
#define C_LONG_MAX __LONG_MAX
#define C_ULONG_MAX (2UL*LONG_MAX+1)
#define C_LLONG_MIN (-LLONG_MAX-1)
#define C_LLONG_MAX  0x7fffffffffffffffLL
#define C_ULLONG_MAX (2ULL*LLONG_MAX+1)

#define C_MB_LEN_MAX 4

/* Some implementations restrict these to posix source, xopensource, gnu source or bsd source... 
 * we should probably follow that behaviour, but in our compatibility library...
 */

#define C_PIPE_BUF 4096
#define C_FILESIZEBITS 64
#ifndef C_NAMEMAX 
#define C_NAMEMAX 4096
#endif /* C_NAMEMAX */
#define C_PATH_MAX 4096
#define C_NGROUPS_MAX 32
#define C_ARG_MAX 131072
#define C_IOV_MAX 1024
#define C_SYMLOOP_MAX 40
#define C_WORD_BIT 32
#define C_SSIZE_MAX LONG_MAX
#define C_TZNAME_MAX 6
#define C_TTY_NAME_MAX 32
#define C_HOST_NAME_MAX 255

#include <bits/limits.h>

#ifdef C_LONG_MAX == 0x7fffffffL
#define C_LONG_BIT 32
#else
#define C_LONG_BIT 64
#endif /* C_LONG_MAX == 0x7fffffffL */

#define C_PTHREAD_KEYS_MAX 128
#define C_PTHREAD_STACK_MIN 2048
#define C_PTHREAD_DESTRUCTOR_ITERATORS 4
#define C_SEM_VALUE_MAX 0x7fffffff
#define C_SEM_NSEMS_MAX 256
#define C_DELAYTIMER_MAX 0x7fffffff
#define C_MQ_PRIO_MAX 32768
#define C_LOGIN_NAME_MAX 256

#define C_BC_BASE_MAX 99
#define C_BC_DIM_MAX 2048
#define C_BC_SCALE_MAX 99
#define C_BC_STRING_MAX 1000
#define C_CHARCLASS_NAME_MAX 14
#define C_COLL_WEIGHTS_MAX 2
#define C_LINE_MAX 4096
#define C_RE_DUP_MAX 255

#define C_NL_ARGS_MAX 9
#define C_NL_MSGMAX 32767
#define C_NL_SETMAX 255
#define C_NL_TEXTMAX 2048

#ifdef PAGESIZE
#define C_PAGE_SIZE PAGESIZE
#endif /* PAGESIZE */

#define C_NZERO 20
#define C_NL_LANGMAX 32

#define C_NL_NMAX 16

/* POSIX */

/* The following definitons are unrelated to the host operating system,
 * they are provided by SUS */

#define _POSIX_AIO_LISTIO_MAX   2
#define _POSIX_AIO_MAX          1
#define _POSIX_ARG_MAX          4096
#define _POSIX_CHILD_MAX        25
#define _POSIX_CLOCKRES_MIN     20000000
#define _POSIX_DELAYTIMER_MAX   32
#define _POSIX_HOST_NAME_MAX    255
#define _POSIX_LINK_MAX         8
#define _POSIX_LOGIN_NAME_MAX   9
#define _POSIX_MAX_CANON        255
#define _POSIX_MAX_INPUT        255
#define _POSIX_MQ_OPEN_MAX      8
#define _POSIX_MQ_PRIO_MAX      32
#define _POSIX_NAME_MAX         14
#define _POSIX_NGROUPS_MAX      8
#define _POSIX_OPEN_MAX         20
#define _POSIX_PATH_MAX         256
#define _POSIX_PIPE_BUF         512
#define _POSIX_RE_DUP_MAX       255
#define _POSIX_RTSIG_MAX        8
#define _POSIX_SEM_NSEMS_MAX    256
#define _POSIX_SEM_VALUE_MAX    32767
#define _POSIX_SIGQUEUE_MAX     32
#define _POSIX_SSIZE_MAX        32767
#define _POSIX_STREAM_MAX       8
#define _POSIX_SS_REPL_MAX      4
#define _POSIX_SYMLINK_MAX      255
#define _POSIX_SYMLOOP_MAX      8
#define _POSIX_THREAD_DESTRUCTOR_ITERATIONS 4
#define _POSIX_THREAD_KEYS_MAX  128
#define _POSIX_THREAD_THREADS_MAX 64
#define _POSIX_TIMER_MAX        32
#define _POSIX_TRACE_EVENT_NAME_MAX 30
#define _POSIX_TRACE_NAME_MAX   8
#define _POSIX_TRACE_SYS_MAX    8
#define _POSIX_TRACE_USER_EVENT_MAX 32
#define _POSIX_TTY_NAME_MAX     9
#define _POSIX_TZNAME_MAX       6
#define _POSIX2_BC_BASE_MAX     99
#define _POSIX2_BC_DIM_MAX      2048
#define _POSIX2_BC_SCALE_MAX    99
#define _POSIX2_BC_STRING_MAX   1000
#define _POSIX2_CHARCLASS_NAME_MAX 14
#define _POSIX2_COLL_WEIGHTS_MAX 2
#define _POSIX2_EXPR_NEST_MAX   32
#define _POSIX2_LINE_MAX        2048
#define _POSIX2_RE_DUP_MAX      255

#define _XOPEN_IOV_MAX          16
#define _XOPEN_NAME_MAX         255
#define _XOPEN_PATH_MAX         1024

#endif /* _LIMITS_H */