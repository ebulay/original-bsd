#	@(#)Makefile	5.3 (Berkeley) 11/14/91

PROG=	hunt
SRCS=	deliv2.c glue4.c glue5.c hunt1.c hunt2.c hunt3.c hunt5.c hunt6.c \
	hunt7.c hunt8.c hunt9.c refer3.c shell.c tick.c
CFLAGS+=-I${.CURDIR}/../refer
DPADD=	${LIBCOMPAT}
LDADD=	-lcompat
BINDIR=	/usr/old/libexec
NOMAN=	noman
.PATH:	${.CURDIR}/../deliv ${.CURDIR}/../refer

.include "../../Makefile.inc"
.include <bsd.prog.mk>
