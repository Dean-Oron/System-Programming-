/******************************************************************************/
/* 					PingPong Between Processes			  					  */
/*					Description -  Using signals for processes communication  */
/*								   Between a process and its child			  */
/*					Author    -     Dean Oron								  */
/*					Date     - 	   30.04.20									  */
/*					Reviewer - 	   Ben Zikri							      */
/*					Open Lab 86											  	  */
/******************************************************************************/
#include <stdio.h>      /* printf() */
#include <stdlib.h>     /* sizeof() */
#include <signal.h>     /* sig_atomic_t, sigaction(), kill() */
#include <string.h>     /* memset() */
#include <unistd.h>     /* pid_t, fork() */


/* #define EXC1 */
/* #define EXC2 */
 #define EXC3 

/****************************************************************************
	   Declaration of the Global variablers and Signal Handler function
****************************************************************************/
sig_atomic_t flag1 = 0;
sig_atomic_t flag2 = 1;

void SignalHandler(int sig);

int main()
{
	char *args[]={"./child", "lets", "check", "whats", "going", "on", NULL};
	pid_t child_pid;

    struct sigaction act;

    memset(&act, 0, sizeof(act));

    act.sa_handler = &SignalHandler;

	/* Sigactions definitions */
	sigaction(SIGUSR1, &act, NULL);

	child_pid = fork();

	    if (0 > child_pid)
	    {
		    printf("ERROR\n");

            return (1);
	    }
	    else if (0 == child_pid) /* The child process is running */
	    {
			if (flag2)
			{
        		execv(args[0], args);
			}
	    }
	    else /* The parent process is running */
	    {
	    	while(1)
	    	{
            	if (flag1)
				{
					printf("PONG\t\t%d\n\n", child_pid);
					flag1 = 0;
					kill(child_pid, SIGUSR2);
				}
	    	}
    }

	return (0);
}

void SignalHandler(int sig)
{
    if (SIGUSR1 == sig)
    {
        flag1 = 1;
    }
    else if (SIGUSR2 == sig)
    {
        flag2 = 1;
    }
}
