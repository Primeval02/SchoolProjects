all: arb8 glsl8

arb8: arb8.c
	gcc arb8.c -Wall -oarb8 -lglut -lGLU -lGL

glsl8: glsl8.cpp
	g++ glsl8.cpp -oglsl8 -lX11 -lGLU -lGL -lGLEW

clean:
	rm -f arb8 glsl8

