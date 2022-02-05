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
 * The following file defines all ERRNOs in the Linux ABI.
 * read the Utopia documentation in order to understand what each one
 * might refer to.
 * 
*/

#define C_EPERM            1
#define C_ENOENT           2
#define C_ESRCH            3
#define C_EINTR            4
#define C_EIO              5
#define C_ENXIO            6
#define C_E2BIG            7
#define C_ENOEXEC          8
#define C_EBADF            9
#define C_ECHILD          10
#define C_EAGAIN          11
#define C_ENOMEM          12
#define C_EACCES          13
#define C_EFAULT          14
#define C_ENOTBLK         15
#define C_EBUSY           16
#define C_EEXIST          17
#define C_EXDEV           18
#define C_ENODEV          19
#define C_ENOTDIR         20
#define C_EISDIR          21
#define C_EINVAL          22
#define C_ENFILE          23
#define C_EMFILE          24
#define C_ENOTTY          25
#define C_ETXTBSY         26
#define C_EFBIG           27
#define C_ENOSPC          28
#define C_ESPIPE          29
#define C_EROFS           30
#define C_EMLINK          31
#define C_EPIPE           32
#define C_EDOM            33
#define C_ERANGE          34
#define C_EDEADLK         35
#define C_ENAMETOOLONG    36
#define C_ENOLCK          37
#define C_ENOSYS          38
#define C_ENOTEMPTY       39
#define C_ELOOP           40
#define C_EWOULDBLOCK     C_EAGAIN
#define C_ENOMSG          42
#define C_EIDRM           43
#define C_ECHRNG          44
#define C_EL2NSYNC        45
#define C_EL3HLT          46
#define C_EL3RST          47
#define C_ELNRNG          48
#define C_EUNATCH         49
#define C_ENOCSI          50
#define C_EL2HLT          51
#define C_EBADE           52
#define C_EBADR           53
#define C_EXFULL          54
#define C_ENOANO          55
#define C_EBADRQC         56
#define C_EBADSLT         57
#define C_EDEADLOCK       C_EDEADLK
#define C_EBFONT          59
#define C_ENOSTR          60
#define C_ENODATA         61
#define C_ETIME           62
#define C_ENOSR           63
#define C_ENONET          64
#define C_ENOPKG          65
#define C_EREMOTE         66
#define C_ENOLINK         67
#define C_EADV            68
#define C_ESRMNT          69
#define C_ECOMM           70
#define C_EPROTO          71
#define C_EMULTIHOP       72
#define C_EDOTDOT         73
#define C_EBADMSG         74
#define C_EOVERFLOW       75
#define C_ENOTUNIQ        76
#define C_EBADFD          77
#define C_EREMCHG         78
#define C_ELIBACC         79
#define C_ELIBBAD         80
#define C_ELIBSCN         81
#define C_ELIBMAX         82
#define C_ELIBEXEC        83
#define C_EILSEQ          84
#define C_ERESTART        85
#define C_ESTRPIPE        86
#define C_EUSERS          87
#define C_ENOTSOCK        88
#define C_EDESTADDRREQ    89
#define C_EMSGSIZE        90
#define C_EPROTOTYPE      91
#define C_ENOPROTOOPT     92
#define C_EPROTONOSUPPORT 93
#define C_ESOCKTNOSUPPORT 94
#define C_EOPNOTSUPP      95
#define C_ENOTSUP         C_EOPNOTSUPP
#define C_EPFNOSUPPORT    96
#define C_EAFNOSUPPORT    97
#define C_EADDRINUSE      98
#define C_EADDRNOTAVAIL   99
#define C_ENETDOWN        100
#define C_ENETUNREACH     101
#define C_ENETRESET       102
#define C_ECONNABORTED    103
#define C_ECONNRESET      104
#define C_ENOBUFS         105
#define C_EISCONN         106
#define C_ENOTCONN        107
#define C_ESHUTDOWN       108
#define C_ETOOMANYREFS    109
#define C_ETIMEDOUT       110
#define C_ECONNREFUSED    111
#define C_EHOSTDOWN       112
#define C_EHOSTUNREACH    113
#define C_EALREADY        114
#define C_EINPROGRESS     115
#define C_ESTALE          116
#define C_EUCLEAN         117
#define C_ENOTNAM         118
#define C_ENAVAIL         119
#define C_EISNAM          120
#define C_EREMOTEIO       121
#define C_EDQUOT          122
#define C_ENOMEDIUM       123
#define C_EMEDIUMTYPE     124
#define C_ECANCELED       125
#define C_ENOKEY          126
#define C_EKEYEXPIRED     127
#define C_EKEYREVOKED     128
#define C_EKEYREJECTED    129
#define C_EOWNERDEAD      130
#define C_ENOTRECOVERABLE 131
#define C_ERFKILL         132
#define C_EHWPOISON       133
