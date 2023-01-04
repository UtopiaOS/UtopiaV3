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

#define __NEED_u32
#define __NEED_i32
#define __NEED_u64
#define __NEED_usize
#define __NEED_size
#define __NEED_Int32
#define __NEED_UniversalType
#define __NEED_USize
#define __NEED_UInt64
#define __NEED_Size
#define __NEED_Void
#define __NEED_Int64
#include <covenant/bits/everytype.h>

typedef Int32 (*CompareFunction)(UniversalType, UniversalType);
typedef UniversalType (*AllocationFunction)(UniversalType, USize, USize);
typedef Size (*IOFunction)(Int32, UniversalType, USize);

typedef Int32 (*ctype_cmpfn)(void *, void *);
typedef void *(*ctype_allocfn)(void *, USize, USize);

#define va_arg(a, b) __builtin_va_arg(a, b)
#define va_copy(a, b) __builtin_va_copy(a, b)
#define va_start(a, b) __builtin_va_start(a, b)
#define va_end(a)   __builtin_va_end(a)

typedef UInt64 (*HashFunction)(const UniversalType, UInt64, UInt64);
typedef Int64 (*ctype_hashfn)(const void *, Int64, Int64);