all: smoke

smoke: smoke.cpp timers.cpp
	g++ smoke.cpp  timers.cpp -Wall -lX11 -lGL -lGLU -lm ./libggfonts.a -osmoke

clean:
	rm -f smoke

