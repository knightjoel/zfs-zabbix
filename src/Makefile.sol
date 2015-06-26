CFLAGS += -Wall -Wextra -std=c99 -pedantic -lzfs -lnvpair 
OBJ = config.o fsutil.o stdprint.o vdev_status.o zio.o memlist.o
PROG=           zio
NO_MAN=         1
SRCS=           zio.c config.c fsutil.c stdprint.c vdev_status.c memlist.c
CC=/opt/gcc-4.8.1/bin/gcc

zio: zio.c $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)    

clean:
	rm -f *.o zio
