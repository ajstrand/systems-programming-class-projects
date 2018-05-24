/*@author: Alex Strand
@computerid: ajstrand
@assignment: lab 9
@class: Systems Programming
A shell to execute simple command line arguments* and do piping if one exists*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#define READ 0
#define WRITE 1
#define BUFFER_SIZE 1<<16
#define ARR_SIZE 1<<16

/*this method parses the arguments you pass to your shell*/
void parse_args(char *buffer, char** args, 
                size_t argument_size, size_t *nargs)
{
    char *buf_args[argument_size]; /* You need C99 */
    char **cp;
    char *wbuf;
    size_t i, j;
    
    wbuf=buffer;
    buf_args[0]=buffer; 
    args[0] =buffer;
    
    for(cp=buf_args; (*cp=strsep(&wbuf, " \n\t")) != NULL ;){
        if ((*cp != '\0') && (++cp >= &buf_args[argument_size]))
            break;
    }
    
    for (j=i=0; buf_args[i]!=NULL; i++){
        if(strlen(buf_args[i])>0)
            args[j++]=buf_args[i];
    }
    
    *nargs=j;
    args[j]=NULL;
}

/*this is where most of the work is done such as checking 
if you want to change directories, forking a process, etc*/
int main(int argc, char *argv[], char *envp[]){
    char buffer[BUFFER_SIZE];
    char *args[ARR_SIZE];
    int fd[2];
    int *ret_status;
    size_t nargs;
    pid_t pid;
    
    while(1){
        printf("$ ");
        fgets(buffer, BUFFER_SIZE, stdin);
        parse_args(buffer, args, ARR_SIZE, &nargs); 

        if (nargs==0) continue;
        if (!strcmp(args[0], "exit" )) exit(0);	  
	if(!strcmp(args[0], "cd"))
		chdir(args[1]);
	if(!strcmp(args[0], "cd .."))
		chdir("..");     
        pid = fork();

        if (pid){
            printf("Waiting for child (%d)\n", pid);
            pid = wait(ret_status);
            printf("Child (%d) finished\n", pid);
        } else {

		if(!strcmp(args[1], "|")){
			pipe(fd);

        		if(fork() != 0){//parent writer
				close (fd[READ]); //close unused end
				dup2 (fd[WRITE], 1); //duplicate used end to stdout
				close(fd[WRITE]); //close original used end
				execlp(args[0], args[0], NULL); //execute writer program
				perror("shell2");
			}
			else{// child, reader
				close (fd[WRITE]);//close unused end
				dup2 (fd[READ], 0); //duplicate used end to stdout
				close (fd[READ]);// close orginakl used end
				execlp(args[2], args[2], NULL);//execute reader program 
				perror("shell2");
			}
		}
	else{
            if( execvp(args[0], args)) {
			
                //puts(strerror(errno));
                exit(127);
            }
	}
        }
    }    
    return 0;
}
