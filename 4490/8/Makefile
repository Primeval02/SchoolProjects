all: verlet balls

verlet: verlet.cpp
	g++ verlet.cpp -Wall -lX11 -lGL -lGLU

balls: balls.cpp
	g++ balls.cpp -o balls -Wall -lX11 -lGL -lGLU

clean:
	rm -f verlet balls a.out
