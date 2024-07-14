# a Makefile is a configuration file for a utility called 'make'
# if you simply run the make command it will look in the current directory for 
# a file called Makefile (this file) and use it to try to build everything for the rule 'all'
# for this class we will generally be trying to compile main1.cpp to runme1 and main2.cpp to runme2
# and so on but that is not always the case
#
# variables used 
# LOGLEVEL 
# used by the cmpslib for the logging , you can pass this in when you call make or it wil
# use the default setting 0 = none, 1=Error only , 2=Errors and Warnings, 3 =everything
# LOG
# the full command for the compiler for the logging option
# COMP
# this will be the compiler we want to use along with all the options
# -Wall     ( with all warnings)
# -std=c++11  ( use c++ 2011 version  of the ANSI standards)


# set a default level for the logging
LOGLEVEL?=3
LOG=-D LOGLEVEL=$(LOGLEVEL) 
COMP=g++ -Wall -std=c++11



# all is the default case , ours is to build "runme"
all:	runme1 
	
# running the command 'make clean' will run this rule and it should clean up all the .o files, log files and other misc stuff
# running make after this will force all the compiled code to be recompiled
# remove anything that ends with a name that starts with runme, ends with ~, ends with .log, ends with .gch and ends with .o

clean:
	rm -f runme* 
	rm -f *~
	rm -f core 
	rm -f *.log 
	rm -f *.gch 
	rm -f *.o 
  

# i will try to explain this single rule
# to build runme its requrements are listed on the first line main.cpp and cmpslib19.h if they are not present the build will fail
# if they are not present it will try to find a rule to build them
# if the timestamp (they are modified) of either main.cpp or cmpslib.h changes it will re compile runme
runme:  main.cpp   cmpslib19.h 
	$(COMP) $(LOG) -o runme main.cpp    



# to enable this make file to serve as a template for all the labs and homeworks it may have some rules that are not used 
runme1:  main1.cpp   cmpslib19.h  
	$(COMP) $(LOG) -o runme1 main1.cpp 

runme2:  main2.cpp   cmpslib19.h 
	$(COMP) $(LOG) -o runme2 main2.cpp    

runme3:  main3.cpp   cmpslib19.h 
	$(COMP) $(LOG) -o runme3 main3.cpp    

runme4:  main4.cpp   cmpslib19.h 
	$(COMP) $(LOG) -o runme4 main4.cpp    

runme5:  main5.cpp   cmpslib19.h 
	$(COMP) $(LOG) -o runme5 main5.cpp    

# this is a generic rule we can use to compile classes and functions that are parts of a compiled appliction
# for example list.h and list.cpp will compile to list.o and linked along with other .o files into a runable appliction
# this will compile foo.cpp and foo.h into foo.o   OR bar.cpp and bar.h into bar.o
%.o: %.cpp %.h
	$(COMP) $(LOG) -c -o $@ $<   

# lets put some code to go get a file if we need it
# some people didnt seem to have the ability to copy a file over from another directory
# so if we put in one of these filenames as a requirment for a build it will jump down here 
# and find a rule to 'Make' it , but for these we will just copy them here 

cmpslib19.h:
		cp /home/fac/msarr/public_files/cmps2020/lib/cmpslib19.h .

fraction.h:
		cp /home/fac/msarr/public_files/cmps2020/lib/fraction.h .


# DO NOT DELETE THIS LINE -- make depend depends on it.




