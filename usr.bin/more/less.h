/*
 * Copyright (c) 1988 Mark Nudleman
 * Copyright (c) 1988, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * %sccs.include.redist.c%
 *
 *	@(#)less.h	8.1 (Berkeley) 06/06/93
 */

#define	RECOMP

#define	NULL_POSITION	((off_t)(-1))

#define	EOI		(0)
#define	READ_INTR	(-2)

/* Special chars used to tell put_line() to do something special */
#define	UL_CHAR		'\201'		/* Enter underline mode */
#define	UE_CHAR		'\202'		/* Exit underline mode */
#define	BO_CHAR		'\203'		/* Enter boldface mode */
#define	BE_CHAR		'\204'		/* Exit boldface mode */

#define	CONTROL_CHAR(c)		(iscntrl(c))
#define	CARAT_CHAR(c)		((c == '\177') ? '?' : (c | 0100))

#define	TOP		(0)
#define	TOP_PLUS_ONE	(1)
#define	BOTTOM		(-1)
#define	BOTTOM_PLUS_ONE	(-2)
#define	MIDDLE		(-3)

#define	A_INVALID		-1

#define	A_AGAIN_SEARCH		1
#define	A_B_LINE		2
#define	A_B_SCREEN		3
#define	A_B_SCROLL		4
#define	A_B_SEARCH		5
#define	A_DIGIT			6
#define	A_EXAMINE		7
#define	A_FREPAINT		8
#define	A_F_LINE		9
#define	A_F_SCREEN		10
#define	A_F_SCROLL		11
#define	A_F_SEARCH		12
#define	A_GOEND			13
#define	A_GOLINE		14
#define	A_GOMARK		15
#define	A_HELP			16
#define	A_NEXT_FILE		17
#define	A_PERCENT		18
#define	A_PREFIX		19
#define	A_PREV_FILE		20
#define	A_QUIT			21
#define	A_REPAINT		22
#define	A_SETMARK		23
#define	A_STAT			24
#define	A_VISUAL		25
#define	A_TAGFILE		26
#define	A_FILE_LIST		27
