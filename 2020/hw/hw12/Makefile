# a Makefile is a configuration file for a utility called 'make'
# if you simply run the make command it will look in the current directory for 
# a file called Makefile (this file) and use it to try to build everything for the rule 'all'
# for this class we will generally be trying to compile main1.cpp to runme1 and main2.cpp to runme2
# and so on but that is not always the case
#
# this make file is a bit complicated but it is the base for all the labs and homeworks so there are 
# parts that may not be used for this particular assignment
#


#create some variables so we can change the value once and use it in our rules

# this will be the compiler we want to use along with all the options
# -Wall     ( with all warnings)
COMP=g++ -Wall 


# comment out this line and make clean to disable logging
LOG=-D LOGGING_ON


# list some files that all the compiled programs need to have present before they can be built
REQUIRED=cmpslib19.h easylogging++.o fraction.h Hash_stu.o 
LINK=easylogging++.o Hash_stu.o



# all is the default case , ours is to build "runme"
all:    runme_int 

# running the command 'make clean' will run this rule and it should clean up all the .o files, log files and other misc stuff
# running make after this will force all the compiled code to be recompiled
# remove anything that ends with a name that starts with runme, ends with ~, ends with .log, ends with .gch and ends with .o
clean:
	rm -f runme* *~ core *.log *.gch *.o

vclean:
	rm -f runme* *~ core *.log *.gch *.o easylogging++.* cmpslib19.h fraction.h



# i will try to explain this single rule
# to build runme its requrements are listed on the first line main.cpp and cmpslib19.h if they are not present the build will fail
# if they are not present it will try to find a rule to build them
# if the timestamp (they are modified) of either main.cpp or cmpslib.h changes it will re compile runme
runme:  main.cpp   $(REQUIRED) 
	$(COMP) $(LOG) -o runme main1.cpp

# to enable this make file to serve as a template for all the labs and homeworks it may have some rules that are not used
runme_int:      main1.cpp   $(REQUIRED)
	$(COMP) $(LOG) -o runme_int main1.cpp $(LINK)





# this is a generic rule we can use to compile classes and functions that are parts of a compiled appliction
# for example list.h and list.cpp will compile to list.o and linked along with other .o files into a runable appliction
# this will compile foo.cpp and foo.h into foo.o   OR bar.cpp and bar.h into bar.o
%.o: %.cpp %.h
	$(COMP) -c -o $@ $<   

# lets put some code to go get a file if we need it
# some people didnt seem to have the ability to copy a file over from another directory
# so if we put in one of these filenames as a requirment for a build it will jump down here 
# and find a rule to 'Make' it , but for these we will just copy them here 

cmpslib19.h:
		cp /home/fac/msarr/public_files/cmps2020/lib/cmpslib19.h .

fraction.h:
		cp /home/fac/msarr/public_files/cmps2020/lib/fraction.h .

easylogging++.h:
		cp /home/fac/msarr/public_files/cmps2020/lib/easylogging++.h .

easylogging++.cc:
		cp /home/fac/msarr/public_files/cmps2020/lib/easylogging++.cc .



# rule to build the logging library funtions
# the two defines are .. 
# dont use the default logfilename as we will set it 
# truncate the logfile it everytime we run
# if the LOGGING_ON is not set above disable all the logging 


ifeq ($(LOG),-D LOGGING_ON)
EL_OPTIONS=-D ELPP_FRESH_LOG_FILE -D ELPP_NO_DEFAULT_LOG_FILE 
	else
EL_OPTIONS=-D ELPP_DISABLE_LOGS -D ELPP_NO_DEFAULT_LOG_FILE 
	endif

easylogging++.o: easylogging++.h easylogging++.cc
	g++ $(EL_OPTIONS) -c easylogging++.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.



