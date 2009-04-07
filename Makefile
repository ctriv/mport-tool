PROG= mport

CFLAGS+=	-I${.CURDIR}/../../lib/libedit/ -I${.CURDIR}/../../lib/libmport
WARNS?= 	3

SRCS=		mport.c query.c delete.c

NO_MAN=		yes

DPADD=  ${LIBTERMCAP} ${LIBEDIT} ${LIBMPORT} 
LDADD=  -ltermcap -ledit -lmport 

.include <bsd.prog.mk>
