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
#define __NEED_usize
#define __NEED_ulong
#define __NEED_u32
#define __NEED_i32
#define __NEED_va_list
#define __NEED_uchar
#define __NEED_ctype_status
#include <covenant/bits/everytype.h>

typedef struct ctype_arr ctype_arr;

struct ctype_arr {
    usize size; // a
    usize length; // n 
    uchar *members; // p
};

/* fmt macros */
enum {
    C_FMTWIDTH = 1 << 0,
    C_FMTLEFT = 1 << 1,
    C_FMTPREC = 1 << 2,
    C_FMTSHARP = 1 << 3,
    C_FMTSPACE = 1 << 4,
    C_FMTSIGN = 1 << 5,
    C_FMTZERO = 1 << 6,
    C_FMTUNSIGNED = 1 << 7,
    C_FMTSHORT = 1 << 8,
    C_FMTLONG = 1 << 9,
    C_FMTVLONG = 1 << 10,
    C_FMTCOMMA = 1 << 11,
    C_FMTBYTE = 1 << 12,
    C_FMTFLAG = 1 << 13,
};

#define c_arr_INIT(a) { sizeof((a)), 0, (a) }

typedef struct ctype_fmt ctype_fmt;
typedef ctype_status (*ctype_fmtopfn)(ctype_fmt *, char *, usize);
typedef ctype_status (*ctype_fmtfn)(ctype_fmt *);

/* ioq macros */
enum {
    /* default file descriptors */
    C_FD0 = 0,
    C_FD1 = 1,
    C_FD2 = 2,

    /* Sizes */
    C_BIOSIZ = 8192,
    C_SMALLBIOSIZ = 512,
    C_ERRSIZ = 512,

    /* Flags */
    C_IOQ_ONOFLUSH = 1 << 0,
    __IOQ_OALL = 1 << 0,
};

struct ctype_fmt {
    ctype_arr *array;
    va_list args;
    ctype_fmtopfn fn;
    usize nfmt;
    i32 prec;
    i32 r;
    i32 width;
    ulong flags;
    void *farg;
};


typedef struct ctype_ioq ctype_ioq;

struct ctype_ioq {
    ctype_arr array;
    ctype_iofn op;
    ctype_fd fd;
};

/* Where do I put you? */
#define C_MIN(a, b) (((a) > (b)) ? (b) : (a))

/* Should you go here? */
#define C_OFLW_UM(a, b, c) ((b) && (c) > (((a)-1)/(b)))

#define c_ioq_INIT(a, b, c) { { sizeof((b)), 0, (b) }, (c), (a) }

/* ioq variables */
extern ctype_ioq *ioq0;
extern ctype_ioq *ioq1;
extern ctype_ioq *ioq2;

/* Non standard errors
 * TODO: Where should these go */
#define C_RUNEERROR 0xFFFD

/* Non standard limits
 * TODO: Where should these go? */
#define C_RUNEMAX 0x10FFFF