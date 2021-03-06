/*-
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
 *
 * %sccs.include.redist.c%
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)memmove.c	5.2 (Berkeley) 05/15/90";
#endif /* LIBC_SCCS and not lint */

#include <string.h>
#include <sys/stdc.h>

/*
 * Copy a block of memory, handling overlap.
 */
void *
memmove(dst, src, length)
	void *dst;
	const void *src;
	register size_t length;
{
	bcopy((const char *)src, (char *)dst, length);
	return(dst);
}
