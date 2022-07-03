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
#ifndef _COVENANT_KERNEL_BASIC_H
#define _COVENANT_KERNEL_BASIC_H

#define __NEED_size
#define __NEED_usize
#define __NEED_ctype_fd
#define __NEED_u32
#define __NEED_i32
#define __NEED_ulong
#define __NEED_va_list
#include <covenant/bits/everytype.h>
#include <covenant/fn.h>
#include <covenant/status.h>


_Noreturn void c_kernel_exit(i32);
size c_kernel_write(ctype_fd, void*, usize);

/* Write to a file descriptor */
size c_kernel_fdwrite(ctype_fd, void*, usize);

ctype_status c_kernel_munmap(void*, size);


i32 c_kernel_open3(char*, i32, u32);

i32 c_kernel_fcntl(ctype_fd, i32, ...);

size c_kernel_read(ctype_fd, void*, usize);

/* This should be in our thread library
 * for now covenant doesn't support threads
 * that is a can of worms nobody wants to open
 * right now...
*/
#define c_kernel_abort() { while (*(volatile i32 *)0); }

ctype_status c_kernel_close(ctype_fd);

// TODO: Should there really be here?
#define C_KERNEL_FGETFL 3
#define C_KERNEL_FGETLK 5
#define C_KERNEL_FSETLK 6
#define C_KERNEL_FSETLKW 7

#define C_KERNEL_FGETFD 1
#define C_KERNEL_FSETFD 2


#endif /* _COVENANT_KERNEL_BASIC_H */