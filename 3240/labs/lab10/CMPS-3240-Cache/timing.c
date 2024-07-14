
/* -----------------------------------------------------------------
   timing.c:	These routines will do timing and measure of resource 
		utilization for you when you run a program. The traditional 
		way to do this is with setitimer() and getitimer() using 
		signals. On some systems these work with MULTI-THREADED 
		applications as well (e.g.  Tru64 Unix on Alphas) but 
		on other systems (e.g.  Solaris on Suns, some Linuxes) 
		only the REAL time is correct and the VIRTual and 
		PROFiling time may be completely erroneous with 
		multithreaded applications. 

		In this case, if the system supports the user of POSIX 
		timers (which support multithreading), you should define 
		the compile time variable USE_POSIX_TIMERS. The types
		of timing are as follows:

		process REAL time	time by the clock (in
				real_sec + real_usec/1000000).

		process VIRTual time	time used ONLY by the
				process itself and NOT including 
				any time used in SYSTEM calls (in
				virt_sec + virt_usec/1000000).

		process PROFiling time	realistic time: includes
				both the time used by the running
				process AS WELL AS the time used by 
				the SYSTEM to provide services the
				process requested (in
				prof_sec + prof_usec/1000000).

		#define DEBUG3 if you want to see timing actions and
		interrupts as they occur.

		#include "timing.h" in your program

		the setup (in main()) is as follows:

		init_timing()
		...
		read_timing()
		fprintf_timing()
		..
		read_timing()
		fprintf_timing()
		...
		pause_timing()
		printf_timing()

   ----------------------------------------------------------------- */

#include <stdio.h>			
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <math.h>
#include <errno.h>
#include <sys/resource.h>

#if defined(__alpha) || defined(__linux) || defined(__bsd)

#elif defined(__sun)		/* need to read /proc file system */
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/procfs.h>		/* defines PR_MSACCT so that rusage
				struct will be accurate */
#define PRIO_MAX	20	/* not defined on Solaris */

#endif

#if defined(USE_POSIX_TIMERS)	/* POSIX timer_settime(), timer_gettime() */

#include <time.h>

static	struct	sigevent	notify_method;	/* this struct is
defined as

struct sigevent {
	int		sigev_notify;	notification mode = SIGEV_SIGNAL
	int		sigev_signo;	signal number = 2
	union sigval	sigev_value;	signal value = timerID
	void		(*sigev_notify_function)(union sigval);
	pthread_attr_t	*sigev_notify_attributes;
	int		__sigev_pad2;
} */

static	timer_t	read_id, virt_id, prof_id;	/* timer id's */
static	struct itimerspec	real, virt, prof,
			oreal, ovirt, oprof;	/* these structs are
defined as 

typedef struct itimerspec {		
	struct timespec	it_interval;
	struct timespec	it_value;
} 
where the fields are defined as

typedef struct  timespec {	
	time_t		tv_sec;	
	long		tv_nsec;
} */
#else				/* BSD UNIX setitimer(), getitimer() */

static	sigset_t	timermask;	/* this is a global because it
			MUST be initialized by init_timing() before
			activation of the handler timer_handler() */

static	struct	itimerval	real, virt, prof,
			oreal, ovirt, oprof;	/* these structs are 
defined as

struct	itimerval {
	struct timeval	it_interval;	
	struct timeval	it_value;
} 
where the fields are defined as

struct	timeval {
	time_t	tv_sec;
	time_t	tv_usec;
} */
#endif

static	long	real_sec, real_usec;	/* accumulated real time */
static	long	virt_sec, virt_usec;	/* accumulated virtual (user process
				only) time */
static	long	prof_sec, prof_usec;	/* accumulated profiling (user 
				process and system work on its behalf) time */

static	int	interrupt_occurred = 0;	/* set by timer_handler to WARN 
				routines like read_timing() that they may 
				have corrupted values */

#define	DONT_RELOAD	     0
#define	START_SEC	     7	/* set timer with 7.999999 seconds */
#define	START_USEC	999999
				/* prototypes */
int update_acctime(time_t countdown_sec, time_t countdown_usec);
int reset_timers(time_t reload_sec, time_t reload_usec,
		time_t countdown_sec, time_t countdown_usec);

/* -----------------------------------------------------------------
   Timer Handler:
			timer_handler()
   ----------------------------------------------------------------- */

void timer_handler(int dummy)
		/* This function handles the REAL interval timer when	
	SIGALRM is triggered, the VIRTual interval timer when
	SIGVTALRM is triggered, and the PROFile interval timer
	when SIGPROF is triggered. */
	{
	int	ret;
	sigset_t	oldmask;

			/* block any timer SIGNAL(s) because we
		are NOT reentrant. Note that the older non-POSIX
		version of this call would be: 

		mask = sigblock( SIGALRM | SIGVTALRM | SIGPROF ); */
	sigprocmask(SIG_BLOCK, &timermask, &oldmask);

			/* check timers and UPDATE the accumulated 
		time(s) */
	getitimer(ITIMER_REAL, &real);
	getitimer(ITIMER_VIRTUAL, &virt);
	getitimer(ITIMER_PROF, &prof);
	ret = update_acctime((time_t)START_SEC, (time_t)START_USEC);
	if (ret) goto finish;	/* timers are ALL zero */

#if defined(DEBUG3)
	fflush(stdout);

	fprintf(stderr, "\n      time(sec.usec): %4ld.%6.6ld REAL  ", 
			real_sec, real_usec);
	fprintf(stderr, "%4ld.%6.6ld VIRT  ", 
			virt_sec, virt_usec);
	fprintf(stderr, "%4ld.%6.6ld PROF", 
			prof_sec, prof_usec);
	fflush(stderr);
#endif
			/* set the three timers with both the
		reload (DONT) and countdown values */
	reset_timers((time_t)DONT_RELOAD, (time_t)DONT_RELOAD, 
		(time_t)START_SEC, (time_t)START_USEC);

finish:
	interrupt_occurred = 1;	/* let others know */

			/* restore SIGNAL(s) handler */
	signal(SIGALRM, timer_handler);
	signal(SIGVTALRM, timer_handler);
	signal(SIGPROF, timer_handler);

			/* restore SIGNAL mask. Note that the 
		older non-POSIX version of this call would be: 

		sigsetmask(mask); */
	sigprocmask(SIG_SETMASK, &oldmask, NULL);
	}  /* end of timer_handler */

/* -----------------------------------------------------------------
   Utilities:
			stop_timers()
			reset_timers()
			update_acctime()
			init_timing()
			read_timing()
			pause_timing()
			printf_timing()
   ----------------------------------------------------------------- */

int stop_timers(void)
		/* This function stops the three timers */
	{
	real.it_value.tv_sec = (time_t)0;
	real.it_value.tv_usec = (time_t)0;
	setitimer(ITIMER_REAL, &real, &oreal);

	virt.it_value.tv_sec = (time_t)0;
	virt.it_value.tv_usec = (time_t)0;
	setitimer(ITIMER_VIRTUAL, &virt, &ovirt);

	prof.it_value.tv_sec = (time_t)0;
	prof.it_value.tv_usec = (time_t)0;
	setitimer(ITIMER_PROF, &prof, &oprof);

	return(0);
	}  /* end of stop_timers */

/* ----------------------------------------------------------------- */

int reset_timers(time_t reload_sec, time_t reload_usec,
		time_t countdown_sec, time_t countdown_usec)
		/* This function sets the three timers with
	regard to reload values (if 0 this means DONT reload)
	and countdown values */
	{
	int	ret;
	real.it_interval.tv_sec = reload_sec;
	real.it_interval.tv_usec = reload_usec;
	real.it_value.tv_sec = countdown_sec;
	real.it_value.tv_usec = countdown_usec;
	ret = setitimer(ITIMER_REAL, &real, &oreal);
	if (ret != 0)
		{
		fprintf(stderr, 
		  "\nreset_timers: fatal error in setitimer(ITIMER_REAL..)");
		perror("reset_timers: ");
		}

			/* set VIRTUALtimer and SIGVTALRM handler */
	virt.it_interval.tv_sec = reload_sec;
	virt.it_interval.tv_usec = reload_usec;
	virt.it_value.tv_sec = countdown_sec;
	virt.it_value.tv_usec = countdown_usec;
	ret = setitimer(ITIMER_VIRTUAL, &virt, &ovirt);
	if (ret != 0)
		{
		fprintf(stderr, 
		  "\nreset_timers: fatal error in setitimer(ITIMER_VIRT..)");
		perror("reset_timers: ");
		}

			/* set PROFtimer and SIGPROF handler */
	prof.it_interval.tv_sec = reload_sec;
	prof.it_interval.tv_usec = reload_usec;
	prof.it_value.tv_sec = countdown_sec;
	prof.it_value.tv_usec = countdown_usec;
	ret = setitimer(ITIMER_PROF, &prof, &oprof);
	if (ret != 0)
		{
		fprintf(stderr, 
		  "\nreset_timers: fatal error in setitimer(ITIMER_PROF..)");
		perror("reset_timers: ");
		}
	return(0);
	}  /* end of reset_timers */

/* ----------------------------------------------------------------- */

int update_acctime(time_t countdown_sec, time_t countdown_usec)
		/* This function updates the variables which
	accumulated total { REAL, VIRT, PROF } time:
			real_sec real_usec
			virt_sec virt_usec
			prof_sec prof_usec 
	The caller must, of course, read the timer values BEFORE
	calling this routine. If ALL of the timer values are AT
	FULL COUNT this routine returns 1, otherwise it returns 0. */
	{
	if ( (real.it_value.tv_sec == countdown_sec) &&
		(real.it_value.tv_usec == countdown_usec) &&
		(virt.it_value.tv_sec == countdown_sec) &&
		(virt.it_value.tv_usec == countdown_usec) &&
		(prof.it_value.tv_sec == countdown_sec) &&
		(prof.it_value.tv_usec == countdown_usec) )
		return(1);		/* nothing to do */

					/* anomalies on some Linuxes */
	if (real.it_value.tv_sec > countdown_sec)
	  {
#if defined(DEBUG3)
	  fflush(stdout);
	  fprintf(stderr, 
		"\n ..correcting REAL from %4ld.%6.6ld to %4ld.%6.6ld ",
		(long)real.it_value.tv_sec, (long)real.it_value.tv_usec, 
		(long)countdown_sec, (long)countdown_usec);
	  fflush(stderr);
#endif
	  real.it_value.tv_sec = countdown_sec;
	  real.it_value.tv_usec = countdown_usec;
	  }
	if (virt.it_value.tv_sec > countdown_sec)
	  {
#if defined(DEBUG3)
	  fflush(stdout);
	  fprintf(stderr, 
		"\n ..correcting VIRT from %4ld.%6.6ld to %4ld.%6.6ld ",
		(long)virt.it_value.tv_sec, (long)virt.it_value.tv_usec, 
		(long)countdown_sec, (long)countdown_usec);
	  fflush(stderr);
#endif
	  virt.it_value.tv_sec = countdown_sec;
	  virt.it_value.tv_usec = countdown_usec;
	  }
	if (prof.it_value.tv_sec > countdown_sec)
	  {
#if defined(DEBUG3)
	  fflush(stdout);
	  fprintf(stderr, 
		"\n ..correcting PROF from %4ld.%6.6ld to %4ld.%6.6ld ",
		(long)prof.it_value.tv_sec, (long)prof.it_value.tv_usec, 
		(long)countdown_sec, (long)countdown_usec);
	  fflush(stderr);
#endif
	  prof.it_value.tv_sec = countdown_sec;
	  prof.it_value.tv_usec = countdown_usec;
	  }


#if defined(DEBUG3)
	fflush(stdout);
	fprintf(stderr, 
  "\n           time left: %4ld.%6.6ld       %4ld.%6.6ld       %4ld.%6.6ld ", 
		(long)real.it_value.tv_sec, (long)real.it_value.tv_usec,
		(long)virt.it_value.tv_sec, (long)virt.it_value.tv_usec,
		(long)prof.it_value.tv_sec, (long)prof.it_value.tv_usec);
	fflush(stderr);
#endif
	
	real_sec = real_sec +
		((long)countdown_sec - (long)real.it_value.tv_sec);
	real_usec = real_usec +
		((long)countdown_usec - (long)real.it_value.tv_usec);
	if (real_usec > 999999)
		{
		real_sec++;
		real_usec = real_usec - 999999;
		}
	virt_sec = virt_sec +
		((long)countdown_sec - (long)virt.it_value.tv_sec);
	virt_usec = virt_usec +
		((long)countdown_usec - (long)virt.it_value.tv_usec);
	if (virt_usec > 999999)
		{
		virt_sec++;
		virt_usec = virt_usec - 999999;
		}
	prof_sec = prof_sec +
		((long)countdown_sec - (long)prof.it_value.tv_sec);
	prof_usec = prof_usec +
		((long)countdown_usec - (long)prof.it_value.tv_usec);
	if (prof_usec > 999999)
		{
		prof_sec++;
		prof_usec = prof_usec - 999999;
		}
	return(0);
	}  /* end of update_acctime */

/* ----------------------------------------------------------------- */

void	init_timing(void)
		/* This function zeroes the accumulated time variables
	(real_sec, virt_sec, prof_sec), sets the signal handler(s)
	and starts the timers counting down in *** NO-RELOAD *** mode */
	{
#if defined(USE_POSIX_TIMERS)
	sigemptyset(&timermask);
	sigaddset(&timermask, SIGEV_SIGNAL);
#else
	sigemptyset(&timermask);
	sigaddset(&timermask, SIGALRM);
	sigaddset(&timermask, SIGVTALRM);
	sigaddset(&timermask, SIGPROF);
#endif

	real_sec = 0L;	
	real_usec = 0L;
	signal(SIGALRM, timer_handler);
	virt_sec = 0L;
	virt_usec = 0L;
	signal(SIGVTALRM, timer_handler);
	prof_sec = 0L;
	prof_usec = 0L;
	signal(SIGPROF, timer_handler);
	reset_timers((time_t)DONT_RELOAD, (time_t)DONT_RELOAD, 
		(time_t)START_SEC, (time_t)START_USEC);

	}  /* end of init_timing */

/* ----------------------------------------------------------------- */

void read_timing(long *preal_sec, long *preal_usec,
		 long *pvirt_sec, long *pvirt_usec,
		 long *pprof_sec, long *pprof_usec)
		/* Read the timers on the fly but keep them running 
	(and DON'T change the global variables: real_sec, real_usec,
	... etc.) Note that we may have to do this code twice if
	the timer_handler() is triggered by an interrupt. */
	{
	long	tmpreal_sec, tmpreal_usec, tmpvirt_sec, tmpvirt_usec,
		tmpprof_sec, tmpprof_usec;
redo:
	if (interrupt_occurred)
		interrupt_occurred = 0;

	getitimer(ITIMER_REAL, &real);
	getitimer(ITIMER_VIRTUAL, &virt);
	getitimer(ITIMER_PROF, &prof);

					/* anomalies on some Linuxes */
	if (real.it_value.tv_sec > START_SEC)
	  {
#if defined(DEBUG3)
	  fflush(stdout);
	  fprintf(stderr, 
		"\n ..correcting REAL from %4ld.%6.6ld to %4ld.%6.6ld ",
		(long)real.it_value.tv_sec, (long)real.it_value.tv_usec, 
		(long)START_SEC, (long)START_USEC);
	  fflush(stderr);
#endif
	  real.it_value.tv_sec = START_SEC;
	  real.it_value.tv_usec = START_USEC;
	  }
	if (virt.it_value.tv_sec > START_SEC)
	  {
#if defined(DEBUG3)
	  fflush(stdout);
	  fprintf(stderr, 
		"\n ..correcting VIRT from %4ld.%6.6ld to %4ld.%6.6ld ",
		(long)virt.it_value.tv_sec, (long)virt.it_value.tv_usec, 
		(long)START_SEC, (long)START_USEC);
	  fflush(stderr);
#endif
	  virt.it_value.tv_sec = START_SEC;
	  virt.it_value.tv_usec = START_USEC;
	  }
	if (prof.it_value.tv_sec > START_SEC)
	  {
#if defined(DEBUG3)
	  fflush(stdout);
	  fprintf(stderr, 
		"\n ..correcting PROF from %4ld.%6.6ld to %4ld.%6.6ld ",
		(long)prof.it_value.tv_sec, (long)prof.it_value.tv_usec, 
		(long)START_SEC, (long)START_USEC);
	  fflush(stderr);
#endif
	  prof.it_value.tv_sec = START_SEC;
	  prof.it_value.tv_usec = START_USEC;
	  }

	tmpreal_sec = real_sec +
		((long)START_SEC - (long)real.it_value.tv_sec);
	tmpreal_usec = real_usec +
		((long)START_USEC - (long)real.it_value.tv_usec);
	if (tmpreal_usec > 999999)
		{
		tmpreal_sec++;
		tmpreal_usec = tmpreal_usec - 999999;
		}
	tmpvirt_sec = virt_sec +
		((long)START_SEC - (long)virt.it_value.tv_sec);
	tmpvirt_usec = virt_usec +
		((long)START_USEC - (long)virt.it_value.tv_usec);
	if (tmpvirt_usec > 999999)
		{
		tmpvirt_sec++;
		tmpvirt_usec = tmpvirt_usec - 999999;
		}
	tmpprof_sec = prof_sec +
		((long)START_SEC - (long)prof.it_value.tv_sec);
	tmpprof_usec = prof_usec +
		((long)START_USEC - (long)prof.it_value.tv_usec);
	if (tmpprof_usec > 999999)
		{
		tmpprof_sec++;
		tmpprof_usec = tmpprof_usec - 999999;
		}

	if (interrupt_occurred)	
		goto redo;		/* complications.. try again 
				since this should not happen twice
				in a row (not with an 8 SEC countdown) */
	*preal_sec = tmpreal_sec;
	*preal_usec = tmpreal_usec;
	*pvirt_sec = tmpvirt_sec;
	*pvirt_usec = tmpvirt_usec;
	*pprof_sec = tmpprof_sec;
	*pprof_usec = tmpprof_usec;
	}  /* end of read_timing */

/* ----------------------------------------------------------------- */

void pause_timing(void)
		/* This function disables SIGNAL(s), reads the timers 
	(we may incur a small error here if the timer ran out just AFTER 
	we disabled its signal), updates the accumulated time(s), and
	stops the timer(s). */
	{
	int	ret;
	signal(SIGALRM, SIG_IGN);
	signal(SIGVTALRM, SIG_IGN);
	signal(SIGPROF, SIG_IGN);
	ret = getitimer(ITIMER_REAL, &real);
	if (ret != 0)
		{
		fprintf(stderr, 
		  "\npause_timing: fatal error in getitimer(ITIMER_REAL..)");
		perror("pause_timing: ");
		}
	ret = getitimer(ITIMER_VIRTUAL, &virt);
	if (ret != 0)
		{
		fprintf(stderr, 
		  "\npause_timing: fatal error in getitimer(ITIMER_VIRT..)");
		perror("pause_timing: ");
		}
	ret = getitimer(ITIMER_PROF, &prof);
	if (ret != 0)
		{
		fprintf(stderr, 
		  "\npause_timing: fatal error in getitimer(ITIMER_PROF..)");
		perror("pause_timing: ");
		}
	update_acctime((time_t)START_SEC, (time_t)START_USEC);
	stop_timers();
	}  /* end of pause_timing */

/* ----------------------------------------------------------------- */

void printf_timing(void)
		/* this function prints out the timing results (after
	you have stopped the timer(s)) to stdout */
	{
	fprintf(stdout, "\nTotal(sec.usec): %4ld.%6.6ld REAL  ", 
			real_sec, real_usec);
	fprintf(stdout, "%4ld.%6.6ld VIRT  ", 
			virt_sec, virt_usec);
	fprintf(stdout, "%4ld.%6.6ld PROF", 
			prof_sec, prof_usec);
	fprintf(stdout, "\n");
	fflush(stdout);
	}


/* ----------------------------------------------------------------- */

void fprintf_timing(FILE *fp, 
			long real_sec, long real_usec,
			long virt_sec, long virt_usec,
			long prof_sec, long prof_usec)
		/* this function prints out the timing results (in
	the same format as printf_timing() above but allows
	directing the output to a file. it assumes that the
	values were obtained by a call to read_timing(). */
	{
	if (fp == NULL)
		{
		fprintf(stderr, "\nfprintf_timing: fatal error: fp = NULL");
		fflush(stderr);
		return;
		}
	fprintf(fp, "\nTotal time(sec.usec): %4ld.%6.6ld REAL  ", 
			real_sec, real_usec);
	fprintf(fp, "%4ld.%6.6ld VIRT  ", 
			virt_sec, virt_usec);
	fprintf(fp, "%4ld.%6.6ld PROF", 
			prof_sec, prof_usec);
	fprintf(fp, "\n");
	fflush(fp);
	}

/* ----------------------------------------------------------------- 
Resource utility functions
   ----------------------------------------------------------------- */

#if defined(__alpha) || defined(__linux) || defined(__bsd)
int get_resource_usage(int who, struct rusage *ptr_rusage)
	{
	return(getrusage(who, ptr_rusage));
	}

#elif defined(__sun)
int get_resource_usage(int who, struct rusage *ptr_rusage)
		/* this function is necessary since the Solaris
	getrusage() fucntion IGNORES most of the fields. one has
	to read the /proc filesystem to get the majority of the
	information. for ALL other systems 
			get_resource_usage() = getrusage() */
	{
	int	tmpfd;
	char	tmpfile_name[256];
	prusage_t	prusage;	/* see /usr/include/sys/procfs.h */
	prpsinfo_t	prpsinfo;	/* see /usr/include/sys/old_procfs.h */

			/* print string "/proc/[pid]" into tmpfile_name[] */
	sprintf(tmpfile_name, "/proc/%05ld", (long)getpid());

			/* open the /proc/[pid] file */
	if ((tmpfd = open(tmpfile_name, O_RDONLY, 0)) < 0)
		{
		perror("get_resource_usage: open:");
		return(-1);		/* failure */
		}
			/* get maximum RSS (resident set size) */
	if (ioctl(tmpfd, PIOCPSINFO, &prpsinfo) < 0)
		{
		perror("get_resource_usage: ioctl(PIOCPSINFO):");
		close(tmpfd);
		return(-1);		/* failure */
		}
	ptr_rusage->ru_maxrss = prpsinfo.pr_rssize * getpagesize();
	ptr_rusage->ru_ixrss = 0;	/* integral RSS */
	ptr_rusage->ru_idrss = 0;	/* integral DATA */
	ptr_rusage->ru_isrss = 0;	/* integral STACK */

			/* get the rest of the information */
	if (ioctl(tmpfd, PIOCUSAGE, &prusage) < 0)
		{
		perror("get_resource_usage: ioctl(PIOCUSAGE):");
		close(tmpfd);
		return(-1);		/* failure */
		}
	close(tmpfd);

	ptr_rusage->ru_utime.tv_sec  = prusage.pr_utime.tv_sec;
	ptr_rusage->ru_utime.tv_usec = prusage.pr_utime.tv_nsec/1000;
	ptr_rusage->ru_stime.tv_sec  = prusage.pr_stime.tv_sec;
	ptr_rusage->ru_stime.tv_usec = prusage.pr_stime.tv_nsec/1000;
	ptr_rusage->ru_minflt = prusage.pr_minf;
	ptr_rusage->ru_majflt = prusage.pr_majf;
	ptr_rusage->ru_nswap  = prusage.pr_nswap;
	ptr_rusage->ru_inblock  = prusage.pr_inblk;
	ptr_rusage->ru_oublock  = prusage.pr_oublk;
	ptr_rusage->ru_nvcsw  = prusage.pr_vctx;
	ptr_rusage->ru_nivcsw  = prusage.pr_ictx;

	return(0);	
	}  /* end of get_resource_usage */
#endif

/* ----------------------------------------------------------------- */

void fprintf_rusage(FILE *fp, struct rusage *pstart, struct rusage *pstop)
		/* this function accepts a FILE pointer, which could
	be stdout or stderr, pointers to a starting rusage structure
	and a stopping rusage structure and prints out the net
	results (for the code executed in between). It also prints the 
	current software pagesize. The number of TLB misses has NOT YET 
	BEEN IMPLEMENTED */
	{
	double	start_usertime, stop_usertime, start_systemtime,
		stop_systemtime;

	start_usertime = (double)(*pstart).ru_utime.tv_sec + 
			(double)((*pstart).ru_utime.tv_usec/1000000.0);
	stop_usertime = (double)(*pstop).ru_utime.tv_sec + 
			(double)((*pstop).ru_utime.tv_usec/1000000.0);
	start_systemtime = (double)(*pstart).ru_stime.tv_sec + 
			(double)((*pstart).ru_stime.tv_usec/1000000.0);
	stop_systemtime = (double)(*pstop).ru_stime.tv_sec + 
			(double)((*pstop).ru_stime.tv_usec/1000000.0);

	fprintf(fp, "\n..user time      = %11.6f seconds", 
		stop_usertime - start_usertime);
	fprintf(fp, "\n..system time    = %11.6f seconds", 
		stop_systemtime - start_systemtime);

#if defined(__alpha) || defined(__linux) || defined(__bsd)
	fprintf(fp, "\n..maximum    RSS = %ld000 bytes", pstop->ru_maxrss);
#elif defined(__sun)
	fprintf(fp, "\n..maximum    RSS = %ld bytes", pstop->ru_maxrss);
#endif
	fprintf(fp, "\n..integral   RSS = %ld ", pstop->ru_ixrss);
	fprintf(fp, "\n..integral  DATA = %ld ", pstop->ru_idrss);
	fprintf(fp, "\n..integral STACK = %ld ", pstop->ru_isrss);
	fprintf(fp, "\n..swaps          = %ld ", 
		pstop->ru_nswap - pstart->ru_nswap);
	fprintf(fp, "\n..inblock ops    = %ld ", 
		pstop->ru_inblock - pstart->ru_inblock);
	fprintf(fp, "\n..outblock ops   = %ld ", 

		pstop->ru_oublock - pstart->ru_oublock);
	fprintf(fp, "\n..software pagesize was=%d ", getpagesize());
	fprintf(fp, "\n..tlb misses=\?\?\? "); 
	fprintf(fp, "\n..page reclaims=%ld ", 
			(long)pstop->ru_minflt - (long)pstart->ru_minflt);
	fprintf(fp, "\n..page faults=%ld ", 
			(long)pstop->ru_majflt - (long)pstart->ru_majflt);
	fprintf(fp, "\n..context switches (voluntary)=%ld ", 
			(long)pstop->ru_nvcsw - (long)pstart->ru_nvcsw);
	fprintf(fp, "\n..context switches (involuntary)=%ld ", 
			(long)pstop->ru_nivcsw - (long)pstart->ru_nivcsw);
	fprintf(fp, "\n");
	fflush(fp);
	}  /* end of fprintf_rusage */

