/*-
 * Copyright (c) 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Arthur David Olson of the National Cancer Institute.
 *
 * %sccs.include.redist.c%
 */

#ifndef lint
static char sccsid[] = "@(#)ialloc.c	8.1 (Berkeley) 06/08/93";
#endif /* not lint */

#ifdef notdef
static char	elsieid[] = "@(#)ialloc.c	8.18";
#endif

/*LINTLIBRARY*/

#include <string.h>
#include <stdlib.h>

#ifdef MAL
#define NULLMAL(x)	((x) == NULL || (x) == MAL)
#else /* !defined MAL */
#define NULLMAL(x)	((x) == NULL)
#endif /* !defined MAL */

#define nonzero(n)	(((n) == 0) ? 1 : (n))

char *	icalloc __P((int nelem, int elsize));
char *	icatalloc __P((char * old, const char * new));
char *	icpyalloc __P((const char * string));
char *	imalloc __P((int n));
char *	irealloc __P((char * pointer, int size));
void	ifree __P((char * pointer));

char *
imalloc(n)
const int	n;
{
#ifdef MAL
	register char *	result;

	result = malloc((size_t) nonzero(n));
	return NULLMAL(result) ? NULL : result;
#else /* !defined MAL */
	return malloc((size_t) nonzero(n));
#endif /* !defined MAL */
}

char *
icalloc(nelem, elsize)
int	nelem;
int	elsize;
{
	if (nelem == 0 || elsize == 0)
		nelem = elsize = 1;
	return calloc((size_t) nelem, (size_t) elsize);
}

char *
irealloc(pointer, size)
char * const	pointer;
const int	size;
{
	if (NULLMAL(pointer))
		return imalloc(size);
	return realloc((void *) pointer, (size_t) nonzero(size));
}

char *
icatalloc(old, new)
char * const		old;
const char * const	new;
{
	register char *	result;
	register	oldsize, newsize;

	newsize = NULLMAL(new) ? 0 : strlen(new);
	if (NULLMAL(old))
		oldsize = 0;
	else if (newsize == 0)
		return old;
	else	oldsize = strlen(old);
	if ((result = irealloc(old, oldsize + newsize + 1)) != NULL)
		if (!NULLMAL(new))
			(void) strcpy(result + oldsize, new);
	return result;
}

char *
icpyalloc(string)
const char * const	string;
{
	return icatalloc((char *) NULL, string);
}

void
ifree(p)
char * const	p;
{
	if (!NULLMAL(p))
		(void) free(p);
}

void
icfree(p)
char * const	p;
{
	if (!NULLMAL(p))
		(void) free(p);
}
