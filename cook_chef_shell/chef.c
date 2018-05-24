/*chef.c - Server for message sending to clients
    @author     Ryan Shelton, Alex Strand
    @id         rmshelton, ajstrand
    @course     CSIS-381: Systems Programming
    @assignment Lab10*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

#define DEFAULT_PROTOCOL   0

unsigned long promptForINETAddress();
unsigned long nameToAddr();

main () {
    int serverFd, clientFd, serverLen, clientLen;
    struct sockaddr_in serverINETAddress;/* Server address */
    struct sockaddr_in clientINETAddress; /* Client address */
    struct sockaddr* serverSockAddrPtr; /* Ptr to server address */
    struct sockaddr* clientSockAddrPtr; /* Ptr to client address */
    unsigned long inetAddress;
    /* Ignore death-of-child signals to prevent zombies */
    signal (SIGCHLD, SIG_IGN);
    serverSockAddrPtr = (struct sockaddr*) &serverINETAddress;
    serverLen = sizeof (serverINETAddress);
    clientSockAddrPtr = (struct sockaddr*) &clientINETAddress;
    clientLen = sizeof (clientINETAddress);
    /* Create a  socket, bidirectional, default protocol */
    serverFd = socket (AF_INET, SOCK_STREAM, DEFAULT_PROTOCOL);
    bzero ((char*)&serverINETAddress,sizeof(serverINETAddress));
    serverINETAddress.sin_family = AF_INET; /* Set domain type */
    serverINETAddress.sin_addr.s_addr = htonl(INADDR_ANY); /* IP */  
	 serverINETAddress.sin_port = htons(15108); /* Set name */
    bind (serverFd, serverSockAddrPtr, serverLen); /* Create file */
    listen (serverFd, 5); /* Maximum pending connection length */
    while (1) /* Loop forever */
        {
        /* Accept a client connection */
        clientFd = accept (serverFd, clientSockAddrPtr, &clientLen);
        //if (fork () == 0) /* Create child to send receipe */
            //{
				dup2(0, clientFd);
				dup2(1, clientFd);
				//dup2(clientFd, 0);
				//dup2(clientFd, 1);
				execvp("shell", NULL);  
				//writeRecipe (clientFd); /* Send the recipe */
            close (clientFd); /* Close the socket */
            exit (/* EXIT_SUCCESS */ 0); /* Terminate */
        //}
        //else
            //close (clientFd); /* Close the client descriptor */
    }
}

/*Data reader
    @param fd - file location to be read from*/
readRecipe (fd)
int fd;
{
    char str[200];
	 //do
	 //{
        while (readLine (fd, str)) /* Read lines until end-of-input */
		  printf (str); /* Echo line from socket */
    //} while (str[0] != 'q' && str[3] != 't');
}

/*Reads a line from a file
     @param fd - file location to be read
     @param str - character array for storage*/
readLine (fd, str)
int fd;
char* str;
/* Read a single NULL-terminated line */
{
    int n;
    do /* Read characters until NULL or end-of-input */
    {
        n = read (fd,str, 1); /* Read one character */
    }
    while (n > 0 && *str++ != '\0');
    return (n > 0); /* Return false if end-of-input */
}

/*Writes a recipe to file
     @param fd - the file to be read*/
writeRecipe (fd)
int fd;
{
    char str[1024];
    printf("Message: ");
    //do
	 //{  
	     fgets(str, 1024, stdin);
        write(fd, str, strlen (str) + 1);
	 //} while (str[0] != 'q' && str[3] != 't');
}