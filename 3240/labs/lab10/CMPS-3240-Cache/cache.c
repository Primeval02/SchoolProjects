
		/* cache.c: This program is a test program which 
	repeatedly (REPEAT times) accesses the same NUM_ENTRIES entries
	in a large array using a FIXED SPACING of line_offset entries
	between accesses (i.e. if tmparray[j] has just been accessed
	the next access will be to tmparray[j + line_offset]). The
	user chooses the value of line_offset (between 1 and getpagesize()).
	You can run this program quickly via

	echo "{line_offset value}" | ./cache

	It is possible to set line_offset so that EVEN a 2-way or 
	4-way set associative cache will be frustrated. The user 
	should try to explain WHY the difference in PROFiling time
	is so great between many close values of line_offset (e.g.
	compare line_offset=4096, a power of 2, and line_offset=4093,
	a prime) even though the RSS is exactly the same in both cases, 
	the same pages in memory are used, very nearly the same entries 
	in the array are accessed, and very nearly the same locality
	with regard to page usage is observed.

	In order to conserve memory and be NICE to other users IT IS
	IMPORTANT THAT the program first lower its priority with 

		nice(PRIO_MAX - 1);

	and set a maximum RSS (resident set size) which will be
	sufficient but not wasteful with

		#define	NEW_MAX_RSS 64000000
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

#define	NEW_MAX_RSS	64000000	/* 64M of core - actually this is
				operating system dependent - Tru64 Unix
				DOESN'T observe this lower limit UNLESS
				core memory is in short supply */

				/* Global variables */
long	real_sec, real_usec;	/* accumulated real time */
long	virt_sec, virt_usec;	/* accumulated virtual (user process
				only) time */
long	prof_sec, prof_usec;	/* accumulated profiling (user process 
				and system work on its behalf) time */

#define	NUM_ENTRIES	8192	/* NUM_ENTRIES accessed in tmparray[] */
#define	REPEAT		1024	/* REPETITIONs */

				/* Function Prototypes */

/* ----------------------------------------------------------------- */

int main(int argc, char *argv[], char *envp[])
	{
	char	answer[128], filename[64];
	int	cache_page, line_offset, value;
	FILE	*logfp;
	int	i, j, k, ret;
	int	*tmparray;	/* for malloc'd static array */
	struct	rusage	r_start, r_stop;

	cache_page = getpagesize();

			/* Ask for line_offset */
again:	fprintf(stdout, "\nEnter line_offset: ");
	fgets(answer, sizeof(answer) - 1, stdin);
	fflush(stdin);
	line_offset = atoi(answer);
	if ( (cache_page < line_offset) || (line_offset < 1) )
		{
		fprintf(stderr,
			"\nline_offset=%d out of range [1, %d]",
				line_offset, cache_page); 
		fflush(stderr);
		goto again;
		}

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
	sprintf(filename, "cache_%4.4d.log", line_offset);
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
	tmparray = (int *)malloc(NUM_ENTRIES * cache_page * sizeof(int));
	if (tmparray == (int *)NULL)
		{
		fprintf(stderr,
			"\nmalloc: fatal error:");
		fflush(stderr);
		exit(-4);
		}
	fprintf(stdout,
		"\nUsing line_offset=%d cache_page=%d..",
			line_offset, cache_page);
	fflush(stdout);
	fprintf(logfp,
		"\nUsing line_offset=%d cache_page=%d..",
			line_offset, cache_page);
	fflush(logfp);

	value = 0;
	for (k = 0 ; k < REPEAT ; k++)
	    {
	    i = 0;	/* only for fprintf() */
    	    j = 0;
	    if ((k % 32) == 0)
		{
		fprintf(stdout,
	"\n..setting entry, tmparry[], value [%9d,%9d,%9d]", i, j, value);
		fflush(stdout);
		}
	    for (i = 0 ; i < NUM_ENTRIES ; i++)
		{
		tmparray[j] = value;	/* tmparray[j] will be kept
				in the L2 cache until cache contention
				with newer data forces it to be written
				out to (slower) main memory */
		j = j + line_offset;
		value++; 

		if  ( ((i % 1024) == 0) && ((k % 32) == 0) )
			{
			putchar(0x08); putchar(0x08);
			putchar(0x08); putchar(0x08);
			putchar(0x08); putchar(0x08);
			putchar(0x08); putchar(0x08);
			putchar(0x08); putchar(0x08);
			putchar(0x08); putchar(0x08);
			putchar(0x08); putchar(0x08);
			putchar(0x08); putchar(0x08);
			putchar(0x08); putchar(0x08);
			putchar(0x08); putchar(0x08);
			putchar(0x08); putchar(0x08);
			putchar(0x08); putchar(0x08);
			putchar(0x08); putchar(0x08);
			putchar(0x08); putchar(0x08);
			putchar(0x08); putchar(0x08);
			fprintf(stdout, "%9d,%9d,%9d]", i, j, value);
			fflush(stdout);
			} 
		}  /* end of for (i = .. */
	    }  /* end of for (k = .. */

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

