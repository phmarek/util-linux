/*
 * Copyright (C) 2014 Ondrej Oprala <ooprala@redhat.com>
 *
 * This file may be distributed under the terms of the
 * GNU Lesser General Public License.
 */
#ifndef UTIL_LINUX_DEBUG_H
#define UTIL_LINUX_DEBUG_H

#define _UL_DEBUG
#ifdef _UL_DEBUG

#include <stdarg.h>

#define UL_DEBUG_DEFINE_MASK(m) int m ## _debug_mask
#define UL_DEBUG_DECLARE_MASK(m) extern UL_DEBUG_DEFINE_MASK(m)

/* p - flag prefix, m - flag postfix */
#define UL_DEBUG_DEFINE_FLAG(p, m) p ## m

/* l - library name, p - flag prefix, m - flag postfix, x - function */
# define __UL_DBG(l, p, m, x) \
			do { \
				if ((p ## m) & l ## _debug_mask) { \
					fprintf(stderr, "%d: %s: %8s: ", getpid(), # l, # m); \
					x; \
				} \
			} while (0)

#define __UL_INIT_DEBUG(lib, pref, mask, env) do { \
	if (lib ## _debug_mask & pref ## INIT) \
	; \
	else if (!mask) { \
		char *str = getenv(# env); \
		if (str) \
			lib ## _debug_mask = strtoul(str, 0, 0); \
	} else \
		lib ## _debug_mask = mask; \
	lib ## _debug_mask |= pref ## INIT; \
	if (lib ## _debug_mask != pref ## INIT) { \
		const char *ver = NULL; \
		__UL_DBG(lib, pref, INIT, ul_debug("library debug mask: 0x%04x", \
				lib ## _debug_mask)); \
		/* ul_get_library_version(&ver); \
		__UL_DBG(lib, pref, INIT, ul_debug("library version: %s", ver));*/ \
	} \
} while (0)

static inline void __attribute__ ((__format__ (__printf__, 1, 2)))
ul_debug(const char *mesg, ...)
{
	va_list ap;
	va_start(ap, mesg);
	vfprintf(stderr, mesg, ap);
	va_end(ap);
	fputc('\n', stderr);
}

static inline void __attribute__ ((__format__ (__printf__, 2, 3)))
ul_debugobj(void *handler, const char *mesg, ...)
{
	va_list ap;

	if (handler)
		fprintf(stderr, "[%p]: ", handler);
	va_start(ap, mesg);
	vfprintf(stderr, mesg, ap);
	va_end(ap);
	fputc('\n', stderr);
}

#else /* !_UL_DEBUG */
# define __UL_DBG(l, p, m, x) do { ; } while (0)
#define __UL_INIT_DEBUG(lib, pref, mask, env) do { ; } while (0)
#endif

#endif /* UTIL_LINUX_DEBUG_H */
