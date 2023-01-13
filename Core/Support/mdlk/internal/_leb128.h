/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright 1996, 1997, 1998, 1999, 2000 John D. Polstra.
 * Copyright 2003 Alexander Kabaev <kan@FreeBSD.ORG>.
 * Copyright 2009-2013 Konstantin Belousov <kib@FreeBSD.ORG>.
 * Copyright 2012 John Marino <draco@marino.st>.
 * Copyright 2014-2017 The FreeBSD Foundation
 * Copyright 2021 Zoe Knox <zoe@pixin.net>
 * All rights reserved.
 *
 * Portions of this software were developed by Konstantin Belousov
 * under sponsorship from the FreeBSD Foundation.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided relocatewith the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// Adapted to Utopia by Diego Magdaleno
// LEB128 implementation

#include <covenant/status.h>
#define __NEED_USize
#define __NEED_UInt64
#define __NEED_Int64
#define __NEED_UInt32
#define __NEED_Int32
#define __NEED_Byte
#include <covenant/bits/everytype.h>

Status mdlk_read_unsigned_leb128(const Byte **p, const Byte *end, UInt64* final) {
    UInt64 result = 0;
    Byte q = 0;
    Int32 bit = 0;
    do {
        if (*p == end) {
            return StatusErr;
        }
        UInt64 slice = **p & 0x7f;
        if (bit > 63) {
            return StatusErr;
        } else {
            result |= (slice << bit);
            bit += 7;
        }
        q = **p & 0x80;
        (*p)++;
    } while (q);
    *final = result;
    return StatusOk;
}

Status mdlk_read_signed_leb128(const Byte **p, const Byte *end, Int64* final) {
    Int64 result = 0;
    Int32 bit = 0;
    Byte byte;
    do {
        if (*p == end) {
            return StatusErr;
        }

        byte = **p;

        (*p)++;

        result |= (((Int64)(byte & 0x7f)) << bit);
        bit += 7;

    } while (byte & 0x80);
    if (((byte & 0x40) != 0) && (bit < 64))
        result |= (~0ULL) << bit;
    
    *final = result;
    return StatusOk;
}