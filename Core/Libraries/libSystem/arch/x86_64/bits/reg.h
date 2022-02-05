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

#undef __WORDSIZE
#define __WORDSIZE 64
#define R15    0
#define R14    1
#define R13    2
#define R12    3
#define RBP    4
#define RBX    5
#define R11    6
#define R10    7
#define R9     8
#define R8     9
#define RAX    10
#define RCX    11
#define RDX    12
#define RSI    13
#define RDI    14
#define ORIG_RAX 15
#define RIP    16
#define CS     17
#define EFLAGS 18
#define RSP    19
#define SS     20
#define FS_BASE 21
#define GS_BASE 22
#define DS     23
#define ES     24
#define FS     25
#define GS     26