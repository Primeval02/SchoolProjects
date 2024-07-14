
			/* timing.h: header file for timer routines */
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/resource.h>

#if defined(__alpha) || defined(__linux)

#elif defined(__sun)		
#define PRIO_MAX	20	/* not defined on Solaris */

#endif

extern	struct	itimerval	real, virt, prof,
				oreal, ovirt, oprof;	/* these structs 
are defined as

struct	itimerval {
	struct timeval	it_interval;	
	struct timeval	it_value;
} 
where the fields are defined as

struct	timeval {
	time_t	tv_sec;
	time_t	tv_usec;
} */

extern	long	real_sec, real_usec;	/* accumulated real time */
extern	long	virt_sec, virt_usec;	/* accumulated virtual (user 
			process only) time */
extern	long	prof_sec, prof_usec;	/* accumulated profiling (user 
			process and system work on its behalf) time */

#define	DONT_RELOAD	     0
#define	START_SEC	     7	/* set timer with 7.999999 seconds */
#define	START_USEC	999999

				/* function prototypes */
void timer_handler(int dummy);
		/* This function handles the REAL interval timer when	
	SIGALRM is triggered, the VIRTual interval timer when
	SIGVTALRM is triggered, and the PROFile interval timer
	when SIGPROF is triggered. */
int stop_timers(void);
		/* This function stops the three timers */
int reset_timers(time_t reload_sec, time_t reload_usec,
		time_t countdown_sec, time_t countdown_usec);
		/* This function sets the three timers with
	regard to reload values (if 0 this means DONT reload)
	and countdown values */
int update_acctime(time_t countdown_sec, time_t countdown_usec); 
		/* This function updates the variables which
	accumulated total { REAL, VIRT, PROF } time:
			real_sec real_usec
			virt_sec virt_usec
			prof_sec prof_usec 
	The caller must, of course, read the timer values BEFORE
	calling this routine. If ALL of the timer values are AT
	FULL COUNT this routine returns 1, otherwise it returns 0. */

void init_timing(void);
		/* This function zeroes the accumulated time variables
	(real_sec, virt_sec, prof_sec), sets the signal handler(s)
	and starts the timers counting down in NO-RELOAD mode */
void read_timing(long *preal_sec, long *preal_usec,
		 long *pvirt_sec, long *pvirt_usec,
		 long *pprof_sec, long *pprof_usec);
		/* Read the timers on the fly but keep them running 
	(and DON'T change the global variables: real_sec, real_usec,
	... etc.) */
		/* Read the timers but keep them running */
void pause_timing(void);
		/* This function disables SIGNAL(s), reads the timers 
	(we may incur a small error here if the timer ran out just AFTER 
	we disabled its signal), updates the accumulated time(s), and
	stops the timer(s). */
void printf_timing(void);
		/* this function prints out the timing results (after
	you have stopped the timer(s)) to stdout */
void fprintf_timing(FILE *fp, 
			long real_sec, long real_usec,
			long virt_sec, long virt_usec,
			long prof_sec, long prof_usec);
		/* this function prints out the timing results (in
	the same format as printf_timing() above but allows
	directing the output to a file. it assumes that the
	values were obtained by a call to read_timing(). */

int get_resource_usage(int who, struct rusage *ptr_rusage);
		/* this function is necessary since the Solaris
	getrusage() fucntion IGNORES most of the fields. one has
	to read the /proc filesystem to get the majority of the
	information. for ALL other systems 
			get_resource_usage() = getrusage() */
void fprintf_rusage(FILE *fp, struct rusage *pstart, struct rusage *pstop);
		/* this function accepts a FILE pointer, which could
	be stdout or stderr, pointers to a starting rusage structure
	and a stopping rusage structure and prints out the net
	results (for the code executed in between). It also prints the 
	current software pagesize. The number of TLB misses has NOT YET 
	BEEN IMPLEMENTED */

