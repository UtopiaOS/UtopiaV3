#ifndef JEMALLOC_H_
#define JEMALLOC_H_

#ifdef __cplusplus
extern "C" {
#endif /* ifdef __cplusplus */

/* Define if __attribute__((...)) syntax is supported */
#define JEMALLOC_HAVE_ATTR

/* Define if alloc_size attribute is supported */
#define JEMALLOC_HAVE_ATTR_ALLOC_SIZE

/* Define if format_arg(...) attribute is supported */
#define JEMALLOC_HAVE_ATTR_FORMAT_ARG

/* Define if format(gnu_printf, ...) attributes is supported */
/* #undef JEMALLOC_HAVE_ATTR_FORMAT_GNU_PRINTF */

/* Define if format(printf, ...) attribute is supported */
/* #define JEMALLOC_HAVE_ATTR_FORMAT_PRINTF */

/* Define if format(c_fmt_print, ...) attribute is supported */
#define JEMALLOC_HAVE_ATTR_COVENANT_PRINTF

/*
 * Define overrides for non-standard allocator-related functions if they are
 * present on the system.
 */
/* #undef JEMALLOC_OVERRIDE_MEMALIGN */
#define JEMALLOC_OVERRIDE_VALLOC

/*
 * At least Linux omits the "const" in:
 *
 *   size_t malloc_usable_size(const void *ptr);
 *
 * Match the operating system's prototype.
 */
#define JEMALLOC_USABLE_SIZE_CONST const

/*
 * If defined, specify throw() for the public function prototypes when compiling
 * with C++.  The only justification for this is to match the prototypes that
 * glibc defines.
 */
/* #undef JEMALLOC_USE_CXX_THROW */

#ifdef _MSC_VER
#  ifdef _WIN64
#    define LG_SIZEOF_PTR_WIN 3
#  else
#    define LG_SIZEOF_PTR_WIN 2
#  endif
#endif

/* sizeof(void *) == 2^LG_SIZEOF_PTR. */
#define LG_SIZEOF_PTR 3

/*
 * Name mangling for public symbols is controlled by --with-mangling and
 * --with-jemalloc-prefix.  With default settings the je_ prefix is stripped by
 * these macro definitions.
 */
#ifndef JEMALLOC_NO_RENAME
#  define je_aligned_alloc aligned_alloc
#  define je_calloc calloc
#  define je_dallocx dallocx
#  define je_free free
#  define je_mallctl mallctl
#  define je_mallctlbymib mallctlbymib
#  define je_mallctlnametomib mallctlnametomib
#  define je_malloc malloc
#  define je_malloc_conf malloc_conf
#  define je_malloc_message malloc_message
#  define je_malloc_stats_print malloc_stats_print
#  define je_malloc_usable_size malloc_usable_size
#  define je_mallocx mallocx
#  define je_smallocx_ea6b3e973b477b8061e0076bb257dbd7f3faa756 smallocx_ea6b3e973b477b8061e0076bb257dbd7f3faa756
#  define je_nallocx nallocx
#  define je_posix_memalign posix_memalign
#  define je_rallocx rallocx
#  define je_realloc realloc
#  define je_sallocx sallocx
#  define je_sdallocx sdallocx
#  define je_xallocx xallocx
#  define je_valloc valloc
#endif

#include "jemalloc_Utopia.h"

#include <stdbool.h> /* Not availible in Utopia yet... */
#include <stdlib.h> /* Poor implementation... */
#include <stdint.h> /* No implementation... */
#include <limits.h> /* Different names... */
#include <strings.h> /* Non existant in Utopia... */

#define JEMALLOC_VERSION "5.2.1-0-gea6b3e973b477b8061e0076bb257dbd7f3faa756"
#define JEMALLOC_VERSION_MAJOR 5
#define JEMALLOC_VERSION_MINOR 2
#define JEMALLOC_VERSION_BUGFIX 1
#define JEMALLOC_VERSION_NREV 0
#define JEMALLOC_VERSION_GID "ea6b3e973b477b8061e0076bb257dbd7f3faa756"
#define JEMALLOC_VERSION_GID_IDENT ea6b3e973b477b8061e0076bb257dbd7f3faa756

#endif