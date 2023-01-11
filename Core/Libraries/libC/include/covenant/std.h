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

/* 
 * TODO: Following that Utopia's libc is kind of simplified 
 * aka that we don't have the same type fragmentation and requirements that 
 * POSIX-like libraries required, should we just consider dropping this?
 * the #define stuff seems useless most of the times due to the design of the library.
 * 
 * Is it a better idea to just drop this and instead use this method of definition in
 * our POSIX compat library?
*/
#define __NEED_size
#define __NEED_u32
#define __NEED_i32
#define __NEED_ushort
#define __NEED_uintptr
#define __NEED_ctype_status
#define __NEED_intptr
#define __NEED_ulong
#define __NEED_ctype_rune
#define __NEED_ctype_error
#define __NEED_uvlong
#define __NEED_u64
#define __NEED_vlong
#define __NEED_umax
#define __NEED_UInt8
#define __NEED_Rune
#define __NEED_UIntPtr
#define __NEED_UVLong
#define __NEED_UInt32
#define __NEED_VLong
#define __NEED_UInt64
#define __NEED_UShort
#define __NEED_IntPtr
#include <covenant/bits/everytype.h>
#include <covenant/kernel/basic.h>
#include <covenant/fn.h>
#include <covenant/dat.h>
#include <covenant/status.h>
#include <covenant/bool.h>

#define nil ((void *)0)

/* Default functions in the std */
UniversalType c_std_bsearch(UniversalType, UniversalType, USize, USize, CompareFunction);
UniversalType c_std_nbsearch(UniversalType, UniversalType, USize, USize, CompareFunction);
UniversalType c_std_realloc(UniversalType, USize, USize);
UniversalType c_std_alloc(USize, USize);
UniversalType c_std_malloc(USize);
Void c_std_setalloc(AllocationFunction);
Void c_std_exit(Int32);
char *c_std_getsyserr(void);
char *c_std_strerror(Int32, char*, USize);
Void c_std_errstr(char *, USize);
UniversalType c_std_free_(UniversalType);

// TODO: Should this stay as a macro?
#define c_std_offsetof(a, b) (ULong)(&(((a *)0)->b))

// TODO: Stay as a macro?
#define c_std_free(a) a = c_std_free_((a))

/* Array manipulation */
UniversalType c_arr_data(Array*);
USize c_arr_len(Array*, USize);
USize c_arr_avail(Array*);
USize c_arr_bytes(Array *);
Status c_arr_trunc(Array*, USize, USize);
Void c_arr_init(Array*, char*, USize);
Status c_arr_push(Array*, UniversalType, USize, USize);
Status c_arr_ready(Array*, USize, USize);

/* Dynamic */
Status c_dyn_push(Array*, UniversalType, USize, USize);
Status c_dyn_ready(Array*, USize, USize);
UniversalType c_dyn_alloc(Array*, USize, USize);
Status c_dyn_insert(Array*, USize, UniversalType, USize, USize);

/* Operations on memory */
void *c_mem_cpy(void*, USize, void*);
void *c_mem_chr(void*, Size, Int32);
void *c_mem_set(void*, USize, Int32);

/* fmt */
Status c_fmt_install(Rune, FormatFunction);
Size c_fmt_fmt(Format*, char*);
Void c_fmt_init(Format*, UniversalType, FormatOperationFunction);
Status c_fmt_nput(Format*, char*, USize);
Status c_fmt_put(Format*, char*);

/* utf8 */
Status c_utf8_checkrune(Rune);
Int32 c_utf8_charntorune(Rune*, char*, USize);
Int32 c_utf8_chartorune(Rune*, char*);

/* ioq */
Status c_ioq_flush(InOutObject *);
Status c_ioq_nput(InOutObject *, char*, USize);
Size c_ioq_vfmt(InOutObject *, char*, VaList);
Size c_ioq_fmt(InOutObject *, char *, ...);
Size c_ioq_feed(InOutObject *);
Size c_ioq_get(InOutObject *, char *, USize);

/* str */
USize c_str_len(char*, USize);
Int32 c_str_cmp(char*, USize, char*);
char* c_str_cpy(char*, USize, char*);
char* c_str_pncpy(char *__restrict, const char *__restrict, Size);
char* c_str_ncpy(char *__restrict, char *__restrict, Size);
char* c_str_chr(char *, USize, Int32);
Bool c_str_equal(char*, USize, char*);

/* UInt32 */
UInt32 c_u32_unpack(char*);
char* c_u32_pack(char*, UInt32);

/* File I/O */
File* c_file_open(const char*, const char*);
Size c_file_feed(File*);
Size c_file_get(File*, char*, USize);
Size c_file_read(UniversalType p, Size, Size, File*);
Status c_file_flush(File*);
Status c_file_nput(File*, char*, USize);
Size c_file_write(UniversalType p, Size, Size, File*);
Status c_file_close(File*);
void c_file_seek(File*, USize);