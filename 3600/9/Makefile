# Makefile for examples week9

all: xlab9 fork-exec myecho myexec nofork-exec sample1

xlab9: xlab9.c
	gcc xlab9.c -oxlab9 -lpthread

fork-exec: fork-exec.c
	gcc fork-exec.c -Wall -o fork-exec

myecho: myecho.c
	gcc myecho.c -Wall -o myecho

myexec: myexec.c
	gcc myexec.c -Wall -o myexec

nofork-exec: nofork-exec.c
	gcc nofork-exec.c -Wall -o nofork-exec

sample1: sample1.c
	gcc sample1.c -Wall -o sample1

clean:	
	rm -f xlab9 fork-exec myecho myexec nofork-exec sample1
