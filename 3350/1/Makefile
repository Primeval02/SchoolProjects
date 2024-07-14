# to compile your project, type make and press enter

all: lab1 click_test

lab1: lab1.cpp log.cpp log.h
	g++ lab1.cpp log.cpp libggfonts.a -Wall -olab1 -lX11 -lGL -lGLU -lm

click_test: lab1.cpp log.cpp log.h
	g++ lab1.cpp log.cpp libggfonts.a -Wall -oclick_test -lX11 -lGL -lGLU -lm -D TESTING

clean:
	rm -f lab1

