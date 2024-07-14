
		/* cache_off.c: This program essentially defeats the
	L2 cache in the case of sleipnir. You can use the program
	cache_info.c to find the cache parameters listed below: 

       line_size  =   64 bytes (16 integer words)
            sets  = 1024
  pages_per_block =   16 pages
    associativity =   16-way set associative (i.e. 16 blocks)
       cache_size = 1024K bytes total

	The program uses the size of each block

	block_size = line_size * sets = 65536 (64K bytes)

	and it repeatedly accesses an array in such a way that the
	set (or index) of the memory access is ALWAYS THE SAME:

	int 	i, index;
	volatile int	tmparray[NUM_ENTRIES];
	int	fixed_set = 7;
	index = fixed_set;	
	for (i = 0 ; i < NUM_ACCESSES ; i++)
		{
		tmparray[index] = i;
		index = index + block_size/sizeof(int);
		if (index >= NUM_ENTRIES)
			index = fixed_set;
		}

	This defeats the L2 cache because we have only 16-way 
	associativity in sleipnir's L2 cache.

	In order to conserve memory and be NICE to other users IT IS
	IMPORTANT THAT the program first lower its priority with 

		nice(PRIO_MAX - 1);

	and set a maximum RSS (resident set size) which will be
	sufficient but not wasteful with

		#define	NEW_MAX_RSS 256000000
		struct	rlimit rlims;
		rlims.rlim_cur = NEW_MAX_RSS;
		rlims.rlim_max = NEW_MAX_RSS;
		setrlimit(RLIMIT_RSS, &rlims);

							Marc  */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <math.h>
#include <sys/resource.h>
#include <errno.h>

#include "timing.h"			/* our timing routines */

#define	NEW_MAX_RSS	256000000	/* 256M of core: NUM_ENTRIES times
				sizeof(int) */

				/* Global variables */
long	real_sec, real_usec;	/* accumulated real time */
long	virt_sec, virt_usec;	/* accumulated virtual (user process
				only) time */
long	prof_sec, prof_usec;	/* accumulated profiling (user process 
				and system work on its behalf) time */

int	fixed_set = 7;
int	block_size = 32768;	/* linesize * sets = 64K */
#define	NUM_ENTRIES	64000000/* NUM_ENTRIES accessed in tmparray[] */
#define NUM_ACCESSES	65536*16*32

				/* Function Prototypes */

/* ----------------------------------------------------------------- */

int main(int argc, char *argv[], char *envp[])
	{
	char	filename[64];
	int	cache_page, value;
	FILE	*logfp;
	int	i, ret, index;

	volatile int	*tmparray;	/* tmparray points to the start
				of an array of volatile int's . "volatile"
				is being used her to make sure that the
				"write" is done (either to the L2 cache or
				to main memory). In general, a volatile 
				variable is:

		1. still L2 cacheable - thread contention is automatically
			resolved by the cache coherancy algorithm, unless

		2. the actual page is declared "lock in core" and 
			"uncacheable" (this is only the case with some
			kernel memory). */

	//struct	rlimit rlims;
	struct	rusage	r_start, r_stop;

	cache_page = getpagesize();

			/* Reduce MAX_RSS to be NICE to other users */
#if defined(__alpha)
	rlims.rlim_cur = NEW_MAX_RSS;		/* soft */
	rlims.rlim_max = NEW_MAX_RSS;		/* hard */
	if(setrlimit(RLIMIT_RSS, &rlims) < 0)
		{
		perror("setrlimit: ");
		exit(-1);
		} 
#endif
			/* Reduce our priority to be NICE to other users 
		PRIO_MAX is 20 on pegasus so we are doing nice(19) */
	if (nice(PRIO_MAX - 1) < 0)
		{
		perror("nice: ");
		exit(-1);
		}

			/* open log file */
	sprintf(filename, "cache_%4.4d.log", block_size);
	logfp = fopen(filename, "w");	
	if (logfp == (FILE *)NULL)
		{
		fprintf(stderr, 
	  "\n\nmain: *** fopen(\"%s\",\"w\") failed ***\n\n", filename);
		fflush(stderr);
		return(-3);
		}

			/* get starting baseline resource usage */
	ret = get_resource_usage(RUSAGE_SELF, &r_start);
	if (ret != 0)
		{
		fprintf(stderr,
		"\n..get_resource_usage(RUSAGE_SELF,..) return %d ", ret);
		fflush(stderr);
		}
			/* initialize and start timers */
	init_timing();

			/* allocate tmparray[] in data segment */
	tmparray = (volatile int *)malloc(NUM_ENTRIES * sizeof(int));
	if (tmparray == (int *)NULL)
		{
		fprintf(stderr,
			"\nmalloc: fatal error:");
		fflush(stderr);
		exit(-4);
		}
	fprintf(stdout,
		"\nUsing line_offset=%d cache_page=%d..",
			block_size, cache_page);
	fflush(stdout);
	fprintf(logfp,
		"\nUsing line_offset=%d cache_page=%d..",
			block_size, cache_page);
	fflush(logfp);

	value = 0;
	index = fixed_set;	
	for (i = 0 ; i < NUM_ACCESSES ; i++)
		{
		value++;		/* keep track of number of writes */
		tmparray[index] = i;
		index = index + block_size/sizeof(int);	
					/* entries in the array are int's.
					   the "set" never changes,
					   thereby defeating the cache since
					   each block of the cache fills 
					   up at this set */
		if (index >= NUM_ENTRIES)
			index = fixed_set;	/* don't go beyond the 
					  array boundaries */
		}
			/* get ending resource usage */
	ret = get_resource_usage(RUSAGE_SELF, &r_stop);
	if (ret != 0)
		{
		fprintf(stderr,
		"\n..get_resource_usage(RUSAGE_SELF,..) return %d ", ret);
		fflush(stderr);
		}

			/* read timers and then stop them */
	read_timing(&real_sec, &real_usec, &virt_sec, &virt_usec,
			&prof_sec, &prof_usec);
	pause_timing();

			/* print out resources used in rusage struct's 
		to both stdout and our logfile */
	fprintf(stdout, "\n..stopping timers ");
	fprintf_rusage(stdout, &r_start, &r_stop);

	fprintf(logfp, "\n..stopping timers ");
	fprintf_rusage(logfp, &r_start, &r_stop);

			/* printout the total times to both stdout and
		our logfile */
	printf_timing();
	fprintf(stdout, "\nNet Memory Bandwidth %8.3f Mbytes/sec: ",
  ((double)(value*sizeof(int))/((double)prof_sec + (double)prof_usec/1000000.0))
		/1000000.0 );
	fprintf(stdout, "\n\n");
	fflush(stdout);

	fprintf_timing(logfp,
	  real_sec, real_usec, virt_sec, virt_usec, prof_sec, prof_usec);
	fprintf(logfp, "\nNet Memory Bandwidth %8.3f Mbytes/sec: ",
  ((double)(value*sizeof(int))/((double)prof_sec + (double)prof_usec/1000000.0))
		/1000000.0 );
	fprintf(logfp, "\n\n");
	fflush(logfp);
	fclose(logfp);	/* close logfile */
	return(0);
	}  /* end of main */

