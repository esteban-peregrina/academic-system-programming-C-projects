#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void segv(int sig) // Handler associated to a segfault signal 
{
	static int segf_counter = 0;
	segf_counter += 1;
	char message[100];
	sprintf(message,"Segmentation fault n. %d (signal %d)\n",segf_counter,sig);
	write(STDOUT_FILENO,message,strlen(message));
	if(segf_counter > 5)
	{
		kill(getpid(),SIGKILL); //kill myself - too many segfaults
		exit(0);
	}
}

int main(int argc, char *argv[])
{
	struct sigaction action;
	unsigned int k;
	int buff[5];
	
	sigemptyset(&action.sa_mask); // empty signal mask
	action.sa_flags = 0;
	action.sa_handler = segv;     // set the handler function
	sigaction(SIGSEGV,&action,0); // catch SIGSEGV

	for(k=0;k<1000;k++)     // this loop is a segfault producer!!
	      {
			buff[k]=k;
		  }
	
	printf("Ouch!");
	return 0;
}
