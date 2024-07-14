# Start of the actual Makefile
FLAGS = -std=c++11 -g  # -g means add debugging info
LOG = -D LOGLEVEL=0 # 0-none, 1-low, 2-med, 3 high


all:	runme1 runme2 runme3 runme4 runme5 runme6 runme7 runme8

clean:
		rm  -f runme* *~  core *.log *.gch  

runme1:	main1.cpp SLinkedList.o 
	g++ $(FLAGS) -o runme1 $(LOG) main1.cpp SLinkedList.o  

runme2:	main2.cpp SLinkedList.o 
	g++ $(FLAGS) -o runme2 $(LOG) main2.cpp SLinkedList.o  

runme3:	main3.cpp DLinkedList.o 
	g++ $(FLAGS) -o runme3 $(LOG) main3.cpp DLinkedList.o  

runme4:	main4.cpp DLinkedList.o 
	g++ $(FLAGS) -o runme4 $(LOG) main4.cpp DLinkedList.o  

runme5:	main5.cpp BST.o
	g++ $(FLAGS) -o runme5 $(LOG) main5.cpp BST.o 

runme6:	main6.cpp BST.o
	g++ $(FLAGS) -o runme6 $(LOG) main6.cpp BST.o 

runme7:	main7.cpp AVL.o 
	g++ $(FLAGS) -o runme7 $(LOG) main7.cpp AVL.o 

runme8:	main8.cpp AVL.o 
	g++ $(FLAGS) -o runme8 $(LOG) main8.cpp AVL.o 

cmpslib19.h:
	cp /home/fac/msarr/public_files/cmps2020/lib/cmpslib19.h



# DO NOT DELETE THIS LINE -- make depend depends on it.




