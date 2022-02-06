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

#ifndef _MALLOC_NP_H_
#define	_MALLOC_NP_H_

#include <features.h>

#define __NEED_size_t

#include <alltypes.h>

void *	__calloc(size_t number, size_t size) __malloc_like;
void *  __malloc(size_t) __malloc_like;
void *  __realloc(void *ptr, size_t size);
void    __free(void *ptr);
void    *__posix_memalign(void **ptr, size_t alignment, size_t size);
void    *__aligned_alloc(size_t alignment, size_t size);
size_t	__malloc_usable_size(const void *ptr);
void    __malloc_stats_print(void (*write_cb)(void *, const char *),
    void *cbopaque, const char *opts);
int     __mallctl(const char* name, void *oldp, size_t *oldenp, void *newp,
        size_t newlen);
int	__mallctlnametomib(const char *name, size_t *mibp, size_t *miblenp);
int	__mallctlbymib(const size_t *mib, size_t miblen, void *oldp,
    size_t *oldlenp, void *newp, size_t newlen);
void	*__mallocx(size_t size, int flags);
void	*__rallocx(void *ptr, size_t size, int flags);
size_t	__xallocx(void *ptr, size_t size, size_t extra, int flags);
size_t	__sallocx(const void *ptr, int flags);
void	__dallocx(void *ptr, int flags);
void	__sdallocx(void *ptr, size_t size, int flags);
size_t	__nallocx(size_t size, int flags);

void	*memalign(size_t, size_t) __malloc_like __alloc_align(1)
	    __alloc_size(2);

#endif /* _MALLOC_NP_H */