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
#define __NEED_u8
#include <covenant/bits/everytype.h>
#include <covenant/kernel/basic.h>
#include <covenant/fn.h>
#include <covenant/dat.h>
#include <covenant/status.h>

#define nil ((void *)0)

/* Default functions in the std */
void *c_std_bsearch(void *, void *, usize, usize, ctype_cmpfn);
void *c_std_nbsearch(void*, void*, usize, usize, ctype_cmpfn);
void *c_std_realloc(void*, usize, usize);
void *c_std_alloc(usize, usize);
void *c_std_malloc(usize);
void c_std_setalloc(ctype_allocfn);
void c_std_exit(i32);
char *c_std_getsyserr(void);
char *c_std_strerror(i32, char*, usize);
void c_std_errstr(char *, usize);
void *c_std_free_(void *);

// TODO: Should this stay as a macro?
#define c_std_offsetof(a, b) (ulong)(&(((a *)0)->b))

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
void *c_mem_cpy(void*, usize, void*);
void *c_mem_chr(void*, size, i32);
void *c_mem_set(void*, usize, i32);

/* fmt */
ctype_status c_fmt_install(ctype_rune, ctype_fmtfn);
size c_fmt_fmt(ctype_fmt*, char*);
void c_fmt_init(ctype_fmt*, void*, ctype_fmtopfn);
ctype_status c_fmt_nput(ctype_fmt*, char*, usize);
ctype_status c_fmt_put(ctype_fmt*, char*);

/* utf8 */
ctype_status c_utf8_checkrune(ctype_rune);
i32 c_utf8_charntorune(ctype_rune*, char*, usize);
i32 c_utf8_chartorune(ctype_rune*, char*);

/* ioq */
ctype_status c_ioq_flush(ctype_ioq *);
ctype_status c_ioq_nput(ctype_ioq *, char*, usize);
size c_ioq_vfmt(ctype_ioq *, char*, va_list);
size c_ioq_fmt(ctype_ioq *, char *, ...);
size c_ioq_feed(ctype_ioq *);
size c_ioq_get(ctype_ioq *, char *, usize);

/* str */
usize c_str_len(char*, usize);
i32 c_str_cmp(char*, usize, char*);
char* c_str_cpy(char*, usize, char*);
char* c_str_pncpy(char *__restrict, const char *__restrict, size);
char* c_str_ncpy(char *__restrict, char *__restrict, size);
char* c_str_chr(char *, usize, i32);

/* u32 */
u32 c_u32_unpack(char*);
char* c_u32_pack(char*, u32);

/* File I/O */
ctype_file* c_file_open(const char*, const char*);
size c_file_feed(ctype_file*);
size c_file_get(ctype_file*, char*, usize);
size c_file_read(void *p, size, size, ctype_file*);
ctype_status c_file_flush(ctype_file*);
ctype_status c_file_nput(ctype_file*, char*, usize);
size c_file_write(void *p, size, size, ctype_file*);
ctype_status c_file_close(ctype_file*);
void c_file_seek(ctype_file*, usize);