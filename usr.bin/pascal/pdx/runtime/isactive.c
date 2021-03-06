/*-
 * Copyright (c) 1980, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * %sccs.include.redist.c%
 */

#ifndef lint
static char sccsid[] = "@(#)isactive.c	8.1 (Berkeley) 06/06/93";
#endif /* not lint */

/*
 * Decide a the given function is currently active.
 */

#include "defs.h"
#include "runtime.h"
#include "frame.rep"
#include "sym.h"
#include "machine.h"
#include "process.h"

BOOLEAN isactive(f)
SYM *f;
{
	if (isfinished(process)) {
		return(FALSE);
	} else {
		if (f == program) {
			return(TRUE);
		}
		return(findframe(f) != NIL);
	}
}
