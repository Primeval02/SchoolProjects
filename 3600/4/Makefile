# Makefile for week-4

all: shared_mem shm_io sigset sigset2 strings xlab4

shared_mem: shared_mem.c
	gcc shared_mem.c -Wall -oshared_mem

shm_io: shm_io.c
	gcc shm_io.c -Wall -oshm_io

sigset: sigset.c
	gcc sigset.c -Wall -osigset

sigset2: sigset2.c
	gcc sigset2.c -Wall -osigset2

strings: strings.c
	gcc strings.c -Wall -ostrings

xlab4:
	gcc xlab4.c -Wall -oxlab4

clean:
	rm -f shared_mem shm_io sigset sigset2 strings xlab4
