/*-
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * %sccs.include.redist.c%
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)cuserid.c	8.1 (Berkeley) 06/04/93";
#endif /* LIBC_SCCS and not lint */

#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

char *
cuserid(s)
	char *s;
{
	register struct passwd *pwd;

	if ((pwd = getpwuid(geteuid())) == NULL) {
		if (s)
			*s = '\0';
		return (s);
	}
	if (s) {
		(void)strncpy(s, pwd->pw_name, L_cuserid);
		return (s);
	}
	return (pwd->pw_name);
}
