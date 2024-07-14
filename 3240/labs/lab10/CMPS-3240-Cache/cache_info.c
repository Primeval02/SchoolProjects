	/* cache_info.c: this program gets the standard information:

		level, type (data, instruction, or unified)
		line_size
		sets
		associativity
		cache_size

	about the caches (L1-instruction/data and L2) on a linux system 
	by reading the system files under the directory:

	/sys/devices/system/cpu/cpu0/cache/index[012]/ 

	it then displays the information on the screen. Note that normally
	index=0 is L1-cache data, index=1 is L1-cache instruction, and 
	index=2 is the L2-cache (unified).  

	It assumes the standard breakdown of an address into,
	for example:

		 ----------------------------------------------------
		| tag      | set or index (10)   | byte number (6)   |
		 ----------------------------------------------------

	If also checks the parameters for consistency, since it
	SHOULD be the case that

		line_size * sets * associativity = cache_size	 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <math.h>
#include <sys/resource.h>
#include <errno.h>

#define CA_DIRECTORY	"/sys/devices/system/cpu/cpu0/cache/"

#define CA_LINE_SIZE		"coherency_line_size"
#define CA_SETS			"number_of_sets"
#define CA_ASSOCIATIVITY	"ways_of_associativity"
#define CA_SIZE			"size"
#define CA_TYPE			"type"
#define	CA_LEVEL		"level"

/* ---------------------------------------------------------------- */

int	main(int argc, char *argv[], char *envp[])
	{
	char	pathname[256];
	int	num, ret;
	FILE	*ftemp;
	int	pagesize;		/* usually 4K-8K */
	int	line_size;		/* number of bytes in a cache line */
	int	sets;			/* number of rows in the cache; "set"
					   is sometimes called "index" */
	int	pages_per_block;	/* (line_size * sets)/pagesize */
	int	level;			/* 1, 2, or 3 (if present) */
	int	associativity;		/* set way associativity */
	int	cache_size;		/* total bytes needed for the cache */
	char	type[256];		/* instruction, data, or unified */
	int	temp1, temp2;
	char	cache_mult;		/* 'K' or 'M' */

	pagesize = getpagesize();	/* get the page size */

					/* go through each cache */
	for (num = 0 ; num <= 3 ; num++)
		{
	/* ---------------------------- .../index[012]/level */
		sprintf(pathname, "%sindex%d/%s",
			CA_DIRECTORY, num, CA_LEVEL);
		ftemp = fopen(pathname, "r");
		if (ftemp == (FILE *)NULL)
			{
			fflush(stdout); fprintf(stderr, 
				"\n *** fatal error: no such file: %s",
				pathname);
			return(-1);
			}
		else
			{
			ret = fscanf(ftemp, "%d", &level);
			if (ret != 1)
				{
				fflush(stdout); fprintf(stderr, 
				"\n *** fatal error: read failed on: %s",
					pathname);
				fclose(ftemp);
				return(-1);
				}
			fclose(ftemp);
			}
	/* ---------------------------- .../index[012]/type */
		sprintf(pathname, "%sindex%d/%s",
			CA_DIRECTORY, num, CA_TYPE);
		ftemp = fopen(pathname, "r");
		if (ftemp == (FILE *)NULL)
			{
			fflush(stdout); fprintf(stderr, 
				"\n *** fatal error: no such file: %s",
				pathname);
			return(-1);
			}
		else
			{
			ret = fscanf(ftemp, "%s", type);
			if (ret != 1)
				{
				fflush(stdout); fprintf(stderr, 
				"\n *** fatal error: read failed on: %s",
					pathname);
				fclose(ftemp);
				return(-1);
				}
			fclose(ftemp);
			}
	/* ---------------------------- .../index[012]/coherency_line_size */
		sprintf(pathname, "%sindex%d/%s",
			CA_DIRECTORY, num, CA_LINE_SIZE);
		ftemp = fopen(pathname, "r");
		if (ftemp == (FILE *)NULL)
			{
			fflush(stdout); fprintf(stderr, 
				"\n *** fatal error: no such file: %s",
				pathname);
			return(-1);
			}
		else
			{
			ret = fscanf(ftemp, "%d", &line_size);
			if (ret != 1)
				{
				fflush(stdout); fprintf(stderr, 
				"\n *** fatal error: read failed on: %s",
					pathname);
				fclose(ftemp);
				return(-1);
				}
			fclose(ftemp);
			}
	/* ---------------------------- .../index[012]/number_of_sets */
		sprintf(pathname, "%sindex%d/%s",
			CA_DIRECTORY, num, CA_SETS);
		ftemp = fopen(pathname, "r");
		if (ftemp == (FILE *)NULL)
			{
			fflush(stdout); fprintf(stderr, 
				"\n *** fatal error: no such file: %s",
				pathname);
			return(-1);
			}
		else
			{
			ret = fscanf(ftemp, "%d", &sets);
			if (ret != 1)
				{
				fflush(stdout); fprintf(stderr, 
				"\n *** fatal error: read failed on: %s",
					pathname);
				fclose(ftemp);
				return(-1);
				}
			fclose(ftemp);
			}
	/* ---------------------------- .../index[012]/ways_of_associativity */
		sprintf(pathname, "%sindex%d/%s",
			CA_DIRECTORY, num, CA_ASSOCIATIVITY);
		ftemp = fopen(pathname, "r");
		if (ftemp == (FILE *)NULL)
			{
			fflush(stdout); fprintf(stderr, 
				"\n *** fatal error: no such file: %s",
				pathname);
			return(-1);
			}
		else
			{
			ret = fscanf(ftemp, "%d", &associativity);
			if (ret != 1)
				{
				fflush(stdout); fprintf(stderr, 
				"\n *** fatal error: read failed on: %s",
					pathname);
				fclose(ftemp);
				return(-1);
				}
			fclose(ftemp);
			}
	/* ---------------------------- .../index[012]/size */
		sprintf(pathname, "%sindex%d/%s",
			CA_DIRECTORY, num, CA_SIZE);
		ftemp = fopen(pathname, "r");
		if (ftemp == (FILE *)NULL)
			{
			fflush(stdout); fprintf(stderr, 
				"\n *** fatal error: no such file: %s",
				pathname);
			return(-1);
			}
		else
			{
			ret = fscanf(ftemp, "%d%c", &cache_size, 
				&cache_mult);
			if (ret != 2)
				{
				fflush(stdout); fprintf(stderr, 
				"\n *** fatal error: read failed on: %s",
					pathname);
				fclose(ftemp);
				return(-1);
				}
			fclose(ftemp);
			}


					/* now print out info */
		pages_per_block = (line_size * sets)/pagesize;

		printf("\nParameters for the L%d cache (%s):", 
				level, type);
		printf("\n");
		printf("\n        line_size= %8d ", line_size);
		printf("\n             sets= %8d ", sets);
		printf("\n  pages per block= %8d (pagesize= %8d)", 
				pages_per_block, pagesize); 
		printf("\n    associativity= %8d-way set associative", 
				associativity);
		printf("\n       cache_size= %8d%c bytes total", cache_size,
				cache_mult);
					/* check consistency of size */
		temp1 = line_size * sets * associativity;
		if (cache_mult == 'K')
			temp2 = cache_size * 1024;
		else if (cache_mult == 'M')
			temp2 = cache_size * 1024 * 1024;
		else
			goto no_test;
		if (temp1 != temp2)
			printf("\n  *** warning: sizes differ %d %d ***", 
			temp1, temp2);
no_test:
		printf("\n");
		}

	printf("\n\n");	
	return(0);			/* successful */
	}  /* end of main */

