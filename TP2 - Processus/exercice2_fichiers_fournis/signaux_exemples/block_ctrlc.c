#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_INT 5000

int main(int argc, char *argv[])
{
	sigset_t signals1, signals2;
	unsigned int i = 0;

	sigemptyset(&signals1);       //empty signals1
	sigaddset(&signals1,SIGINT);  //select SIGINT
	sigprocmask(SIG_SETMASK,&signals1,0); //block the signals selected by signals1

	printf("PID %d ; starting to count - cannot block me with CTRL+C\n", getpid());
	while(i < MAX_INT)
	{
		printf("\r%d going to %d, can't stop me  ",MAX_INT,i); 
		usleep(1000); //wait 1 ms
		i = i+1;
	}

	sigpending(&signals2);     //retrieve pending signals
	if(sigismember(&signals2,SIGINT))
		printf("\nSIGINT is pending : 'CTRL C' was pressed\n");

	sigemptyset(&signals1);    //empty signals1
	sigprocmask(SIG_SETMASK,&signals1,0); //unblock SIGINT

	printf("\nSIGINT unblocked and not used.\n");
	return 0;
}
