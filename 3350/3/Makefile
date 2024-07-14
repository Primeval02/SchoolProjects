all: lab3getc lab3getcpp

lab3getc: lab3get.c 
	gcc lab3get.c -Wall -olab3getc -lssl -lcrypto 

lab3getcpp: lab3get.cpp 
	g++ lab3get.cpp -Wall -olab3getcpp -lssl -lcrypto 

clean:
	rm -f lab3getc lab3getcpp
