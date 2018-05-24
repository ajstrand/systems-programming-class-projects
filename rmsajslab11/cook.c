/*chef.c - Server for remote linux commands
    @author     Ryan Shelton, Alex Strand
    @id         rmshelton, ajstrand
    @course     CSIS-381: Systems Programming
    @assignment Lab11
	 @notes      Only can be closed thorugh ^C*/
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DEFAULT_PROTOCOL    0

unsigned long promptForINETAddress();
unsigned long nameToAddr();

main (){
    int clientFd, serverLen, result;
    struct sockaddr_in serverINETAddress;
    struct sockaddr* serverSockAddrPtr;
    unsigned long inetAddress;
    serverSockAddrPtr = (struct sockaddr*) &serverINETAddress;
    serverLen = sizeof (serverINETAddress);
    while (1) /* Loop until break */
    {
        inetAddress = promptForINETAddress (); /* Get 32-bit IP */
        if (inetAddress == 0) break; /* Done */
        /* Start by zeroing out the entire address structure */
        bzero ((char*)&serverINETAddress,sizeof(serverINETAddress));
        serverINETAddress.sin_family = AF_INET; /* Use Internet */
        serverINETAddress.sin_addr.s_addr = inetAddress; /* IP */
        serverINETAddress.sin_port = htons (15125);
        /* Now create the client socket */
        clientFd = socket (AF_INET, SOCK_STREAM, DEFAULT_PROTOCOL);
        do /* Loop until a connection is made with the server */
        {
            result = connect (clientFd,serverSockAddrPtr,serverLen);
            if (result == -1) sleep (1); /* Try again in 1 second */
        }
        while (result == -1);
		  do {			
		      writeRecipe (clientFd);
		      readRecipe (clientFd); /* Read the recipe from the server */
		  } while (1);
		  close (clientFd); /* Close the socket */
    }
exit (/* EXIT_SUCCESS */ 0);
}

/*Data reader
    @param fd - file location to be read from*/
readRecipe (fd)
int fd;
{
    char str[1024];
readLine (fd, str); /* Read lines until end-of-input */
 }
 
/*Writes a recipe to file
     @param fd - the file to be read*/
writeRecipe (fd)
int fd;
{
	 char str[1024];
    printf("$ ");
    scanf ("%s", str);
	 write(fd, str, strlen (str) + 1);
}

/*Reads a line from a file
     @param fd - file location to be read
     @param str - character array for storage*/
readLine (fd, str)
int fd;
char* str;
/* Read a single NULL-terminated line */
{
	 char *save = str;
    int n;
	 char EOT = 4;
    do /* Read characters until NULL or end-of-input */
    {
        n = read (fd,str, 1); /* Read one character */
		  printf("%c", *str);
    }
    while (n > 0 && *str != EOT && *str++ != '\0');
    return (n > 0); /* Return false if end-of-input */
}

/*Prompts the user for a INET address
     @return an INET address*/
unsigned long promptForINETAddress ()
{
   char hostName [100]; /* Name from user: numeric or symbolic */
   unsigned long inetAddress; /* 32-bit IP format */
   /* Loop until quit or a legal name is entered */
   /* If quit, return 0 else return host's IP address */
   do
   {
       printf ("Host name (q = quit, s = self): ");
       scanf ("%s", hostName); /* Get name from keyboard */
       if (strcmp (hostName, "q") == 0) return (0); /* Quit */
       inetAddress = nameToAddr (hostName); /* Convert to IP */
       if (inetAddress == 0) printf ("Host name not found\n");
   }
   while (inetAddress == 0);
   return (inetAddress);
}

/*Converts a string into an INET address
     @param name - character array to be used
     @return an INET address*/
unsigned long nameToAddr (name)
char* name;
{
    char hostName [100];
    struct hostent* hostStruct;
    struct in_addr* hostNode;
    /* Convert name into a 32-bit IP address */
    /* If name begins with a digit, assume it's a valid numeric */
    /* Internet address of the form A.B.C.D and convert directly */
    if (isdigit (name[0])) return (inet_addr (name));
    if (strcmp (name, "s") == 0) /* Get host name from database */
    {
        gethostname (hostName,100);
        printf ("Self host name is %s\n", hostName);
    }
    else /* Assume name is a valid symbolic host name */
        strcpy (hostName, name);
    /* Now obtain address information from database */
    hostStruct = gethostbyname (hostName);
    if (hostStruct == NULL) return (0); /* Not Found */
    /* Extract the IP Address from the hostent structure */
    hostNode = (struct in_addr*) hostStruct->h_addr;
    /* Display a readable version for fun */
    printf ("Internet Address = %s\n", inet_ntoa (*hostNode));
    return (hostNode->s_addr); /* Return IP address */
}





