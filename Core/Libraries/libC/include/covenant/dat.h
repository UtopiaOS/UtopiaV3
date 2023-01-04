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
#ifndef __COVENANT_DAT_H__
#define __COVENANT_DAT_H__

#define __NEED_USize
#define __NEED_Byte
#define __NEED_UniversalType
#define __NEED_VaList
#define __NEED_ULong
#define __NEED_FileDescriptor
#define __NEED_SByte
#define __NEED_UChar
#define __NEED_VLong
#define __NEED_UVLong
#define __NEED_Int32
#define __NEED_Size
#define __NEED_UInt32
#include <covenant/bits/everytype.h>
#include <covenant/status.h>

typedef ULong Device;
typedef ULong INode;

typedef struct Array Array;

struct Array {
    USize size;
    USize capacity;
    Byte *members;
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

typedef struct Format Format;
typedef Status (*FormatOperationFunction)(Format *, char *, USize);
typedef Status (*FormatFunction)(Format*);

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

struct Format {
    VaList args;
    FormatOperationFunction fn;
    USize nfmt;
    Int32 prec;
    Int32 r;
    Int32 width;
    ULong flags;
    UniversalType farg;
};


typedef struct InOutObject InOutObject;

typedef struct File File;

struct File {
    Array data;
    IOFunction read_fn;
    IOFunction write_fn;
    FileDescriptor fd;
};

struct InOutObject {
    Array array;
    IOFunction op;
    FileDescriptor fd;
};

typedef struct HST HST;
typedef struct HMD HMD;

struct HMD {
    void (*init)(HST *);
    void (*update)(HST *, char *, USize);
    void (*end)(HST*, char *);
};

struct HST {
    union {
        UInt32 x32[16]; // Buffer for 32 bit hashes
        UInt32 x64[8]; // Buffer for 64 bit hashes
    } st;
    UChar buf[128]; // Buffer for hashing
    USize curlen; // Lenght of the cursor
    USize len; // Lenght of the hash
};

typedef struct ClassicTime ClassicTime;
typedef struct Stat Stat;

struct ClassicTime {
    VLong seconds;
    UVLong nanoseconds;
};

struct Stat {
    Device dev;
    INode inode;
    ULong nlink;
    UInt32 mode;
    UInt32 uid;
    UInt32 gid;
    UInt32 __pad0;
    ULong rdev;
    ULong size;
    ULong block_size;
    ULong blocks;
    ClassicTime accessed_at;
    ClassicTime modified_at;
    ClassicTime changed_at;
    long __unused[3];
};

/* Where do I put you? */
#define C_MIN(a, b) (((a) > (b)) ? (b) : (a))

/* Should you go here? */
#define C_OFLW_UM(a, b, c) ((b) && (c) > (((a)-1)/(b)))

#define c_ioq_INIT(a, b, c) { { sizeof((b)), 0, (b) }, (c), (a) }

#define C_NELEM(a)  (sizeof((a))/sizeof((a)[0]))

/* ioq variables */
extern InOutObject *ioq0;
extern InOutObject *ioq1;
extern InOutObject *ioq2;

/* Non standard errors
 * TODO: Where should these go */
#define C_RUNEERROR 0xFFFD
#define C_ECSTM 0x66D7C8BA
#define C_NILOBJ 0x24D8CA

/* Non standard limits
 * TODO: Where should these go? */
#define C_UTFMAX 6
#define C_RUNEMAX 0x10FFFF
#define C_CHARBIT 8

#endif