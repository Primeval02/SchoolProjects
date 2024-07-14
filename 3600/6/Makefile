# Makefile for week-6 consumer/producer

all: echo create_thread mine strong-writer reader-writer threads1 sample1 sample2 sample3 xlab6

echo: echo.c
	gcc -g echo.c -Wall -oecho -lpthread

sample1: sample1.c
	gcc sample1.c -Wall -osample1 -lpthread

sample2: sample2.c
	gcc sample2.c -Wall -osample2 -lpthread

sample3: sample3.c
	gcc sample3.c -Wall -osample3 -lpthread

create_thread: create_thread.c
	gcc create_thread.c -Wall -ocreate_thread -lpthread

mine: mine.c
	gcc mine.c -Wall -omine -lpthread

reader-writer: reader-writer.c
	gcc reader-writer.c -Wall -oreader-writer -lpthread

strong-writer: strong-writer.c
	gcc strong-writer.c -Wall -ostrong-writer -lpthread

threads1: threads1.c
	gcc threads1.c -Wall -othreads1 -lpthread

xlab6: xlab6.c
	gcc xlab6.c -Wall -oxlab6 -lpthread

clean:
	rm -f echo create_thread mine reader-writer strong-writer
	rm -f threads1 sample1 sample2 sample3 log
	
