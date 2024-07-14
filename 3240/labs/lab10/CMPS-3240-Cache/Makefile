# M. Thomas, D. Danforth, A. Cruz
# 2018

CFLAGS = -Wall -funsigned-char
CC_linux=cc
LD_linux= -lpthread -lm
CC=g++

all: cache cache_info cache_off

clean:
	rm -f *.o *.out

#
# ----------------------------------------------------------------
# Semaphore & Queue Routines
# ----------------------------------------------------------------
#

queue:	queue.o 
	${CC} $@.o -o $@ ${LFLAGS} ${LIBS}
queue.o:	tmpfiles queue.c 
	${CC} -c ${CFLAGS} $*.c

#
# ----------------------------------------------------------------
# Sample resource usage program 
# ----------------------------------------------------------------
#

resource:	resource.o timing.o
	${CC} timing.o $@.o -o $@ ${LFLAGS} ${LIBS}
resource.o:	tmpfiles resource.c timing.h
	${CC} -c ${CFLAGS} $*.c

#
#
# ----------------------------------------------------------------
# Sample use of SIGALRM, setjmp, and longjmp 
# ----------------------------------------------------------------
#

alarm:	alarm.o 
	${CC} $@.o -o $@ ${LFLAGS} ${LIBS}
alarm.o:	tmpfiles alarm.c 
	${CC} -c ${CFLAGS} $*.c

#
#
# ----------------------------------------------------------------
# Sample memory bandwidth test programs
# ----------------------------------------------------------------
#

#align:	align.o timing.o
#	${CC} timing.o $@.o -o $@ ${LFLAGS} ${LIBS}
#align.o:	tmpfiles align.c timing.h
#	${CC} -c ${CFLAGS} $*.c

cache:		cache.o timing.o
	${CC} timing.o cache.o -o cache.out ${CFLAGS}
cache.o:	cache.c timing.h
	${CC} -c ${CFLAGS} cache.c

cache_info:		cache_info.o
	${CC} cache_info.o -o cache_info.out 
cache_info.o:	cache_info.c 
	${CC} -c ${CFLAGS} cache_info.c

cache_off:		cache_off.o timing.o
	${CC} timing.o cache_off.o -o cache_off.out
cache_off.o:	cache_off.c timing.h
	${CC} -c ${CFLAGS} cache_off.c
#
# ----------------------------------------------------------------
# Timer Routines (timer01 timer02 and timer03 are *** OBSOLETE ***)
# ----------------------------------------------------------------
#

timing.o:	timing.c 
	${CC} -c ${CFLAGS} timing.c

#argus:		argus.o timing.o
#	${CC} timing.o $@.o -o $@ ${LFLAGS} ${LIBS}
#argus.o:	tmpfiles argus.c timing.h
#	${CC} -c ${CFLAGS} $*.c

#squared:	squared.o timing.o
#	${CC} ${OPTDFLAGS} timing.o $@.o -o $@ ${LFLAGS} ${LIBS}
#squared.o:	tmpfiles squared.c timing.h
#	${CC} -c ${OPTDFLAGS} $*.c

#petersen:	petersen.o 
#	${CC} ${OPTDFLAGS} $@.o -o $@ ${LFLAGS} ${LIBS}
#petersen.o:	tmpfiles petersen.c 
#	${CC} -c ${OPTDFLAGS} $*.c

#timer04:	timer04.o timing.o
#	${CC} timing.o $@.o -o $@ ${LFLAGS} ${LIBS}
#timer04.o:	tmpfiles timer04.c timing.h
#	${CC} -c ${CFLAGS} $*.c

#
# ----------------------------------------------------------------
# Memory and Paging Routines (pager01 is *** OBSOLETE ***)
# ----------------------------------------------------------------
#

pager01:	pager01.o 
	${CC} $@.o -o $@ ${LFLAGS} ${LIBS}
pager01.o:	tmpfiles pager01.c 
	${CC} -c ${CFLAGS} $*.c

pager02:	pager02.o timing.o
	${CC} timing.o $@.o -o $@ ${LFLAGS} ${LIBS}
pager02.o:	tmpfiles pager02.c timing.h
	${CC} -c ${DFLAGS} ${CFLAGS} $*.c

#
# ----------------------------------------------------------------
# Primes and Pseudo Primes (only on Alpha):
# ----------------------------------------------------------------
#

pseudo:		pseudo.o timing.o
	${CC} ${OPTDFLAGS} timing.o $@.o -o $@ ${LFLAGS} ${LIBS}
pseudo.o:	tmpfiles pseudo.c timing.h
	${CC} -c ${OPTDFLAGS} $*.c

#
# ----------------------------------------------------------------
# Directory and Hardware Device Routines (only on Alpha)
# ----------------------------------------------------------------
#

g_stdio.o:	tmpfiles g_stdio.c g_stdio.h 
	${CC} -c ${CFLAGS} $*.c 

device:		device.o g_stdio.o
	${CC} ${DFLAGS} $@.o g_stdio.o -o $@ ${LFLAGS} ${LIBS}
device.o:	tmpfiles device.c g_stdio.h 
	${CC} -c ${DFLAGS} ${CFLAGS} $*.c

g_timing.o:	tmpfiles g_timing.c 
	${CC} -c ${CFLAGS} $*.c

buffer:		buffer.o g_stdio.o g_timing.o
	${CC} ${DFLAGS} $@.o g_stdio.o g_timing.o -o $@ ${LFLAGS} ${LIBS}
buffer.o:	tmpfiles buffer.c g_stdio.h g_timing.h
	${CC} -c ${DFLAGS} ${CFLAGS} $*.c


