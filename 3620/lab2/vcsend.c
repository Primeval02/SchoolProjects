// Ailand Parriott, Bradley Tate
// CMPS 3620
// Lab 2
// vsend.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>      /* for gethostname() */
#include <arpa/inet.h>   /* for IP address structures and functions */
#include <netdb.h>       /* for gethostbyname() */


int main(int argc, char *argv[], char *envp[])
{
	int	msgsock;  /* The data socket for the client */
	struct	sockaddr_in	name;
	struct	hostent	*hp;  /* Used to resolve hostname to IP address */
	char	chrline[80];
	char	buf[2048];
	int	i, ret, length, size;

  /* We must give the hostname and portnumber on the command line. This
   * checks that the user has typed both. */
	if (argc < 3)
	{
		printf("error: Invoke this program via: \n");
		printf("       vcsend <hostname> <portnumber> \n");
		return (-1);
	}

	if (argc > 1) 
	{
		size = atoi(argv[1]);
		if (size < 1  ||  size > sizeof(buf) - 1) 
			size = sizeof(buf) - 1;
	} else
		size = sizeof(buf) - 1;  /* Default size */

  /* Since we are allowing a domain name as the hostname, we must call
   * gethostbyname() to convert the hostname into an IP address. */
	hp = gethostbyname(argv[1]);
	if (hp == NULL)
	{
		printf("sender: gethostbyname() failed.\n");
		printf("        Are you sure the hostname is correct? \n");
		return (-1);
	}
	printf("\nsender: hostname has been resolved.\n");

  /* Copy the resolved IP address over into the name structure along with 
   * the portnumber given on the command line */
  memcpy((char *)&name.sin_addr, (char *)hp->h_addr, hp->h_length);
	name.sin_family = AF_INET;
	name.sin_port = htons((short)atoi(argv[2]));	


  /* Allocate the data socket and attempt to connect to vcrec. Exit the 
   * program if either socket() or connect() fails. */
	printf("sender: Attempting to connect to server.\n");
	msgsock = socket(AF_INET, SOCK_STREAM, 0);
	if (msgsock < 0)
	{
		perror("sender: socket() failed. ");
		return (-1);
	}

	ret = connect(msgsock, (struct sockaddr *)&name, sizeof name);
	if (ret != 0)
	{
		perror("sender: connect() failed. ");
		fprintf(stderr, 
		"\n        are you sure that the portnumber is correct? \n");
		fflush(stderr);
		return (-1);
	}

  /* Wait for the handshake from vcrec before continuing */
	printf("sender: Waiting for handshake from vcrec...\n");
	sleep(1);	/* pause for clean screen display */
	fflush(stdout);
	recv(msgsock, chrline, sizeof(chrline) - 1, 0); /* Handshake character */
	printf("sender: Handshake received. Entering data loop.\n");

	printf("\nEnter a line of characters at the prompt.\n");
	printf("To exit the program, type just a period and hit enter.\n");

  	/* Go into an infinite loop. We'll use the "break" statement to exit
   	* the loop if the user types a period or send() fails */
   	int repeatSend = 0;
   	while(1)
	{
	// ===================
	// SEND TO VCREC =====
		printf("enter line> ");
		/* Read input from user */
		fgets(chrline, sizeof(chrline) - 1, stdin);
		/* Check for the period to end the connection and also convert any
		* newlines into null characters */
		for (i = 0 ; i < (sizeof(chrline) - 1) ; i++)
			if ( (chrline[i] == '\n') || (chrline[i] == '\0') )
				break;
		if (chrline[i] == '\n')
			chrline[i] = '\0'; 	/* get rid of newline */
		length = strlen(chrline);
		if (chrline[0] == '.')		/* end of stream */
		{	printf("sender: termination requested \n");
			break; /* Exit out of loop */
		}
		ret = send(msgsock, chrline, length + 1, 0);
		if (ret < 0)
		{	perror("sender: write() failed. ");
			break;  /* Exit out of loop */
		}
	// END SEND ==========
	// ===================

	// --------------
	// RECV ---------
		if (msgsock == -1)
			perror("receiver: accept() failed. ");
		else
		{	fflush(stdout);
			do 
			{	bzero(buf,sizeof buf);	
				ret = recv(msgsock, buf, size, 0);
				if (ret < 0)
					perror("receiver: recv() failed. ");
				if (ret == 0)
				{	printf("        received-->sender has ended connection \n");
					repeatSend = 1;
					break;
				}
				else
				{	printf("        received-->%s \n",buf);
					break;
				}
			} while (ret != 0);
			if (repeatSend == 1)
				break;
		}
	// END RECV -----
	// --------------
	}

// ===================
// CLOSE =============

  /* Closing the data socket from vcsend will trigger recv() on vcrec to
   * return 0, which will also cause vcrec to close its sockets and exit */
	close(msgsock);

	return (0);
}  /* end of main */

