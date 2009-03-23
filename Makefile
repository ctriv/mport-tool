PROG= mport

CFLAGS+=	-I${.CURDIR}/../../lib/libedit/ -I${.CURDIR}/../../lib/libmport
WARNS?= 	3

SRCS=		mport.c query.c

NO_MAN=		yes

DPADD=  ${LIBTERMCAP} ${LIBEDIT} ${LIBSQLITE3} ${LIBMPORT} ${LIBARCHIVE} ${LIBBZ2} ${LIBZ}
LDADD=  -ltermcap -ledit -lsqlite3 -lmport -lbz2 -larchive -lz


.include <bsd.prog.mk>
