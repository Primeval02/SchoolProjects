all: xproj1 xproj3

xproj1: xproj1.c
	gcc xproj1.c -Wall -lX11 -oxproj1 -lpthread

xproj3: xproj3.c
	gcc xproj3.c -Wall -lX11 -oxproj3 -lpthread

clean:
	rm -f *.o
	rm -f xproj1 xproj3
