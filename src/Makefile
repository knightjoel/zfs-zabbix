SRCS=           zio.c config.c fsutil.c stdprint.c vdev_status.c memlist.c dedup_stats.c
OBJ=            config.o fsutil.o stdprint.o vdev_status.o zio.o memlist.o dedup_stats.o
PROG=           zio
NO_MAN=         1 

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),SunOS)
CFLAGS +=       -Wall -Wextra -std=c99 -pedantic -lzfs -lnvpair

endif
ifeq ($(UNAME_S),FreeBSD)
CFLAGS+=        -I/usr/src/cddl/contrib/opensolaris/lib/libzpool/common
CFLAGS+=        -I/usr/src/cddl/compat/opensolaris/include
CFLAGS+=        -I/usr/src/cddl/compat/opensolaris/lib/libumem
CFLAGS+=        -I/usr/src/sys/cddl/compat/opensolaris
CFLAGS+=        -I/usr/src/cddl/contrib/opensolaris/head
CFLAGS+=        -I/usr/src/cddl/contrib/opensolaris/lib/libuutil/common
CFLAGS+=        -I/usr/src/cddl/contrib/opensolaris/lib/libzfs/common
CFLAGS+=        -I/usr/src/cddl/contrib/opensolaris/lib/libumem/common
CFLAGS+=        -I/usr/src/cddl/contrib/opensolaris/lib/libnvpair
CFLAGS+=        -I/usr/src/cddl/contrib/opensolaris/lib/libzfs_core/common
CFLAGS+=        -I/usr/src/sys/cddl/contrib/opensolaris/uts/common
CFLAGS+=        -I/usr/src/sys/cddl/contrib/opensolaris/uts/common/fs/zfs
CFLAGS+=        -I/usr/src/sys/cddl/contrib/opensolaris/uts/common/sys
CFLAGS+=        -DNEED_SOLARIS_BOOLEAN -DBSD

DPADD=          ${LIBZFS} ${LIBGEOM} ${LIBBSDXML} ${LIBSBUF} \
                ${LIBM} ${LIBNVPAIR} ${LIBUUTIL} ${LIBUTIL}
LDADD=          -lzfs -lgeom -lbsdxml -lsbuf \
                -lm -lnvpair -luutil -lutil

endif
ifeq ($(UNAME_S),Linux)
CFLAGS+= 	-Wall -Wextra -std=c99 -pedantic -lzfs -lnvpair -DLINUX
CFLAGS+=        -I/usr/include/libspl
CFLAGS+=        -I/usr/include/libzfs

endif 

all: zio.c $(OBJ)
	$(CC) $(CFLAGS) $(LDADD) $(DPADD) -o $(PROG) $(OBJ) 

install:
	install -D -m755 -oroot -groot $(PROG) $(DESTDIR)/usr/local/bin/$(PROG)

clean:
	rm -f *.o zio
