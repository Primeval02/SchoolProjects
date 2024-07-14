// Ailand Parriott, Bradley Tate
// CMPS 3620
// Lab 2
// vcrec.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>      /* for gethostname() */
#include <arpa/inet.h>   /* for IP address structures and functions */

void closeSend(int msgsockF, int sockF)
{
	printf("receiver: ending session also and exiting. \n");
	close(msgsockF);		/* close data socket */
	close(sockF);      /* close listen socket */
}

int main(int argc, char *argv[], char *envp[])
{
	int	sock, msgsock;  /* Sockets are integer file descriptors on Linux */
	struct	sockaddr_in	name, caller;
	char	chrline[80];
	char	buf[2048];
	int	size, length, ret, k, i;

  /* Process the command line for the buffer size, if given */
	if (argc > 1)
	{
		size = atoi(argv[1]);
    /* Validate that the given argument is between 1 and sizeof(buf) - 1 
     * Set to the default size if argument is invalid */
		if (size < 1  ||  size > sizeof(buf) - 1) 
			size = sizeof(buf) - 1;
	}
	else
		size = sizeof(buf) - 1;  /* Default size */

// -----------------------
// CREATE LISTEN ---------

  /* Create the listen socket. This is a TCP socket, so use SOCK_STREAM 
   * Exit if the socket cannot be created */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) 
	{
		perror("receiver: socket() failed. ");
		return (-1);
	}

// END CREATE LISTEN -----
// -----------------------


  /* Bind the socket to an IP address and port. We will use the IP address
   * INADDR_ANY, which tells the system to assign the IP address, and the
   * port number 0, which tells the system to assign a random port number.
   *
   * First we have to set the fields in the sockaddr_in object "name" and then
   * we can call bind(). Again, exit if bind() fails. */
	name.sin_family = AF_INET;         /* TCP/IP family */
	name.sin_addr.s_addr = INADDR_ANY; /* INADDR_ANY = assigned by system */
	name.sin_port = htons(0);	         /* 0 = assigned by system */
	ret = bind(sock,(struct sockaddr *)&name,sizeof name);
	if (ret < 0)
	{
		perror("receiver: bind() failed. ");
		return (-1);
	}

  /* In order to use vcsend to send data to this program, we need to know
   * what port number the system just assigned this program. So this segment
   * calls getsockname() to update the sockaddr_in object "name" with the
   * system assigned values and then print that info to the screen. */
	length = sizeof name;
	ret = getsockname(sock, (struct sockaddr *)&name, (socklen_t *)&length);
	if (ret < 0)
	{
		perror("receiver: getsockname() failed. ");
		return (-1);
	}

	sleep(1);	/* pause for clean screen display */
	printf("\nreceiver: process id: %d ", (int)getpid());
	printf("\nreceiver: IP address: %d.%d.%d.%d",
        (ntohl(name.sin_addr.s_addr) & 0xff000000) >> 24,
        (ntohl(name.sin_addr.s_addr) & 0x00ff0000) >> 16,
        (ntohl(name.sin_addr.s_addr) & 0x0000ff00) >>  8,
        (ntohl(name.sin_addr.s_addr) & 0x000000ff));
	printf("\nreceiver: port number: %hu", ntohs(name.sin_port));
	printf("\n");
	fflush(stdout);

  /* Now we will call listen() and wait for a client to connect. The
   * accept() function will block until there is a client or an error. */

// -----------------------
// LISTEN FOR VCSEND -----
	listen(sock,5);		/* up to 5 clients can connect. only 1st is accepted */
	k = sizeof caller;
	msgsock = accept(sock, (struct sockaddr *)&caller, (socklen_t *)&k);
// END LISTEN ------------
// -----------------------

// --------------
// RECV ---------
	/* We only reach this point when there is an error or a client. We can 
	* check the value of msgsock (the data socket) to see which has happened */
	if (msgsock == -1)
		perror("receiver: accept() failed. ");
	else
	{	printf("\nreceiver: Valid connection received.\n");
		printf("receiver: Sending handshake.\n");
		fflush(stdout);
		/* let vcsend know we are ready by sending a single character */
		buf[0]= '0';		
		send(msgsock, buf, 1, 0);
		printf("receiver: Waiting for client....\n"); 

		int repeatRec = 0;
		while(1)
		{	do	
			{	bzero(buf,sizeof buf);	/* zero buffer to remove old data */
				/* recv() will block until the client sends information, the client
				* closes the connection or an error occurs on the data socket. */
				ret = recv(msgsock, buf, size, 0);
				if (ret < 0)
					perror("receiver: recv() failed. ");
				if (ret == 0)
				{	printf("        received-->sender has ended connection \n");
					repeatRec = 1;
					break;
				}
				else
				{	printf("        received-->%s \n",buf);
					break;
				}
			} while (ret != 0); /* Exit loop only when client ends connection */
			if (repeatRec == 1)
				break;
		// ===================
		// SEND TO VCREC =====
			printf("enter line> ");
			fgets(chrline, sizeof(chrline) - 1, stdin);
			for (i = 0 ; i < (sizeof(chrline) - 1) ; i++)
				if ( (chrline[i] == '\n') || (chrline[i] == '\0') )
					break;
			if (chrline[i] == '\n')
				chrline[i] = '\0'; 
			length = strlen(chrline);
			if (chrline[0] == '.')	
			{	printf("sender: termination requested \n");
				break; /* Exit out of loop */
			}
			ret = send(msgsock, chrline, length + 1, 0);
			if (ret < 0)
			{	perror("sender: write() failed. ");
				break;  /* Exit out of loop */
			}
			if (chrline[0] == '.')	
				break; /* Exit out of loop */
		// END SEND ==========
		// ===================
		}
	}
// END RECV -----
// --------------

// ---------------------------
// CLOSE DATA AND LISTEN -----
	closeSend(msgsock, sock);

	return (0);
}  /* end of main */
