# Makefile: 
#        It uses the GNU C++ Compiler with debugging
#        will create executalbe for 2 programs called clock1 and clock2
# ----------------------------------------------------------------------
# the compiler
CPP = g++ 
#
# compiler flags
CFLAGS = -L/usr/lib/cmplrs/cxx -DPOSIX_4D9   -w -g
#
# this linker flag will link in the math library libm.a 
LFLAGS = -lm
#
RM = rm -f
#
# -----------------------------------------------------------------------
# Explanation of macros:
#     $< is any dependent file which is out of file1
#     $@ is the target's fullname
#
# add suffix .cpp since it is not a default with make utility 
.SUFFIXES:	.cpp .o
#
#
all:	clock1 clock2 fixed fixme

clock1:	clock1.cpp 
	g++ -g -o clock1 clock1.cpp 
#
clock2:	clock2.cpp 
	g++ -g -o clock2 clock2.cpp 
#
fixed: fixed.cpp
	g++ -g -o fixed fixed.cpp
#
fixme: fixme.cpp
	g++ -g -o fixme fixme.cpp
#
clean:
	${RM} core clock1 clock2 fixed fixme log err

