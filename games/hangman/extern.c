/*
 * Copyright (c) 1983, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * %sccs.include.redist.c%
 */

#ifndef lint
static char sccsid[] = "@(#)extern.c	8.1 (Berkeley) 05/31/93";
#endif /* not lint */

# include	"hangman.h"

bool	Guessed[26];

char	Word[BUFSIZ],
	Known[BUFSIZ],
	*Noose_pict[] = {
		"     ______",
		"     |    |",
		"     |",
		"     |",
		"     |",
		"     |",
		"   __|_____",
		"   |      |___",
		"   |_________|",
		NULL
	};

int	Errors,
	Wordnum = 0;

double	Average = 0.0;

ERR_POS	Err_pos[MAXERRS] = {
	{  2, 10, 'O' },
	{  3, 10, '|' },
	{  4, 10, '|' },
	{  5,  9, '/' },
	{  3,  9, '/' },
	{  3, 11, '\\' },
	{  5, 11, '\\' }
};

FILE	*Dict = NULL;

off_t	Dict_size;
