# makefile
# compile to an object file
# then link the object file with libraries needed

all: xlab7 mutex_test dotprod

#link the object files...
xlab7: xlab7.o
	gcc xlab7.o -oxlab7 -lpthread -lm
mutex_test: mutex_test.o
	gcc mutex_test.o -omutex_test -lpthread -lm
dotprod: dotprod.o
	gcc dotprod.o -odotprod -lpthread -lm


#create the object files by compiling...
xlab7.o: xlab7.c
	gcc xlab7.c -c -Wall
mutex_test.o: mutex_test.c
	gcc mutex_test.c -c -DPOSIX_4D9 -Wall
dotprod.o: dotprod.c
	gcc dotprod.c -c -DPOSIX_4D9 -Wall

clean:
	rm -f *.o
	rm -f xlab7
	rm -f dotprod mutex_test
	rm -f out

