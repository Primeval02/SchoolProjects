CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: fasteroids

fasteroids: asteroids_fullscreen.cpp log.cpp timers.cpp
	g++ $(CFLAGS) asteroids_fullscreen.cpp log.cpp timers.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -ofasteroids

clean:
	rm -f fasteroids

