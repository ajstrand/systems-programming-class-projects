/*chef.c - Server for message sending to clients
    @author     Ryan Shelton, Alex Strand
    @id         rmshelton, ajstrand
    @course     CSIS-381: Systems Programming
    @assignment Lab11*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

#define DEFAULT_PROTOCOL   0
#define READ 0
#define WRITE 1
#define BUFFER_SIZE 1<<16
#define ARR_SIZE 1<<16

unsigned long promptForINETAddress();
unsigned long nameToAddr();
void  stoarg(char *string, char **arg);
void shell(int file);

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
	 serverINETAddress.sin_port = htons(15125); /* Set name */
    bind (serverFd, serverSockAddrPtr, serverLen); /* Create file */
    listen (serverFd, 5); /* Maximum pending connection length */
    while (1) /* Loop forever */
    {
        /* Accept a client connection */
        clientFd = accept (serverFd, clientSockAddrPtr, &clientLen);
	     dup2(clientFd, 1);
		  shell(clientFd);  
        close (clientFd); /* Close the socket */
        exit (/* EXIT_SUCCESS */ 0); /* Terminate */
    }
}

/*Data reader
    @param fd - file location to be read from*/
readRecipe (fd)
int fd;
{
    char str[200];
        while (readLine (fd, str)) /* Read lines until end-of-input */
		  printf (str); /* Echo line from socket */
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
	     fgets(str, 1024, stdin);
        write(fd, str, strlen (str) + 1);
}

/*Runs a virtual shell
	@param file - the file location to be read*/
void shell(int file) {
    int pid, status, childPid;
    char string[500], *arg[30];
	 char EOT = 4;
    while (1) {
		  fflush(stdout);
		  readLine(file, string);
        stoarg(string, arg);
        if(strcmp(arg[0], "exit") ==0)
            exit(17);
        pid = fork();
        if (pid != 0) {
            childPid = wait(&status);
				printf("%c", EOT);
        } else {
		      fprintf(stderr, "Exec");
            execvp(*arg, arg);
            printf("ERROR: Failed to Execute\n"); 
        }
    }
}

/**Converts a string to an argument list
    @param *string - sting to be converted
    @param **arg - arg list to be edited*/
void  stoarg(char *string, char **arg)
{
     while (*string != '\0') {
         while (*string == ' ' || *string == '\t' || *string == '\n')
             *string++ = '\0';
         *arg++ = string;
         while (*string != '\0' && *string != ' ' && *string != '\t' 
			    && *string != '\n') 
             string++;
     }
     *arg = '\0';
}