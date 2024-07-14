
all: runner

runner: runner.cpp timers.cpp
	g++ runner.cpp timers.cpp libggfonts.a -Wall \
		-lGL -lGLU -lX11 -lpthread -o runner

clean:
	rm -f runner

