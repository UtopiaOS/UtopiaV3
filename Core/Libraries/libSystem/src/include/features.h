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
#ifndef FEATURES_H
#define FEATURES_H

#include "../../include/features.h"

#define weak __attribute__((__weak__))
#define hidden __attribute__((__visibility__("hidden")))

/*
 * The following code is... ehhh.. untested and weird to say at least
 * Utopia uses the Mach-O format as its default binary format (does libcovenant also targets Mach-O)
 * despite how great the format might be for a lot of things, weak aliasing isn't one of his 
 * strong sides, weak aliasing on Mach-O is almost non existant? Or not supported? Or not used?
 * documentation is extremely unclear, nobody documents Mach-O, probably there is some internal 
 * Apple document which documents all the features of the format?, it looks like there is a 
 * space in Mach-O files where you would put aliases? in the symbol table of each Mach-O file there
 * is a N_INDR which is exactly what we want!, but isn't materialized? isn't implemented?
 * in the Linker? the blog we got this information from is from 2011: http://blog.omega-prime.co.uk/2011/07/06/the-sad-state-of-symbol-aliases/
 * and no info was made ever since, there is an stackoverflow question! with 8 upvotes
 * surely someone replied, well no https://stackoverflow.com/questions/49308950/indirect-symbols-in-mach-o-files
 * there is no reply, the information is weak.
 * 
 * So does it have weak aliases? God knows, reexporting symbols is a feature but it is unclear how this
 * would help?
 * 
 * But there is a workaround? sorta? During 2017 our friend Michael Clark started...
 * well lets say discussing this idea on the llvm mailing lists, and about how lld 
 * (which can't even statically linkMach-Os...) should support this feature. 
 * After a long discussion with apple employees that ended up in nothing
 * he posted some code for weak aliases? do they work? who knows!, i guess time will tell.
 */
#if defined (__UTOPIA__)
#define weak_alias(old, new) \
        __asm__(".globl _", #new); \
        __asm__("_" #new " =  _" #old); \
        extern __typeof (old) new __attribute__((weak_import))
#else
#define weak_alias(old, new) \
    extern __typeof(old) new __attribute__((__weak__, __alias__(#old)))
#endif

#endif /* FEATURES_H */