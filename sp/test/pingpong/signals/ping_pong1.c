/******************************************************************************/
/* 					PingPong Between Processes			  					  */
/*					Description -  Using signals for processes communication  */
/*					Author    -     Dean Oron								  */
/*					Date     - 	   30.04.20									  */
/*					Reviewer - 	   Ben Zikri							      */
/*					Open Lab 86											  	  */
/******************************************************************************/

#include <signal.h> /* siginfo(), SIGUSR, sigaction(), sig_atomic_t */
#include <stdio.h>	/* printf */
#include <string.h> /* memset() */
#include <time.h> 	/* sleep() */
#include <unistd.h> /* getpid() */

/* those flags will open the gate of execution to print 'ping & pong' */
sig_atomic_t flag1 = 0;
sig_atomic_t flag2 = 1;

/* Signal Handlers */
void SignalHandler(int sig);

int main()
{
	pid_t child_pid;

    struct sigaction act;

    memset(&act, 0, sizeof(act));

    act.sa_handler = &SignalHandler;

	/* Sigactions definitions */
	sigaction(SIGUSR1, &act, NULL);
	sigaction(SIGUSR2, &act, NULL);

	child_pid = fork();
    
    while (1)
    {
	    if (0 > child_pid)
	    {
		    printf("ERROR\n");

            return (1);
	    }
        /* This will keep happan as long the child is running */
	    else if (0 == child_pid) 
	    {
            sleep(1);
			if (flag1)
			{
				printf("PING PID is - %d\n\n", (int)getppid());
				flag1 = 0;
				kill(getppid(), SIGUSR2);
			}
	    }

	    else 
	    {
            if (flag2)
			{
				printf("PONG PID is - %d\n\n", child_pid);
				flag2 = 0;
				kill(child_pid, SIGUSR1);
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