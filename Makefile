CFLAGS=-ansi -Wpedantic -Wall -Werror -D_THREAD_SAFE -D_REENTRANT -D_POSIX_C_SOURCE=200112L
LIBRARIES=-lpthread
LFLAGS=

all: incrocio.exe

incrocio.exe: incrocio.o DBGpthread.o
	gcc ${LFLAGS} -o incrocio.exe incrocio.o DBGpthread.o ${LIBRARIES}

incrocio.o: incrocio.c DBGpthread.h
	gcc -c ${CFLAGS} incrocio.c

DBGpthread.o: DBGpthread.c printerror.h
	gcc -c ${CFLAGS} DBGpthread.c

.PHONY: clean run

clean:
	rm -f *.exe *.o *~ core

run: incrocio.exe
	./incrocio.exe


