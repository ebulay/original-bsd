#	@(#)Makefile	8.1 (Berkeley) 06/01/93

PROG=	register
SRCS=	register.c des_rw.c
CFLAGS+=-DCRYPT -DDEBUG -DKERBEROS -I${.CURDIR}/../include
.PATH:	${.CURDIR}/../../usr.bin/rlogin
DPADD=	${LIBKRB} ${LIBDES}
LDADD=	-lkrb -ldes
BINDIR=	/usr/bin
BINOWN=	root
BINMODE=4555

.include <bsd.prog.mk>
