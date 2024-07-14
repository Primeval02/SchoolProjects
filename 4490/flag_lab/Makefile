all: flag

flag: flag.cpp log.cpp
	g++ flag.cpp log.cpp libggfonts.a -Wall -oflag -lX11 -lGL -lGLU -lm

clean:
	rm -f flag
