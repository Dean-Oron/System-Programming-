/******************************************************************************/
/* 					PingPong Between Processes			  					  */
/*					Description -  Using signals for processes communication  */
/*								   Between a process and its child			  */
/*					Author    -    Dean Oron								  */
/*					Date     - 	   30.04.20									  */
/*					Reviewer - 	   Ben Zikri							      */
/*					Open Lab 86											  	  */
/******************************************************************************/
#include <stdio.h>      /* printf() */
#include <stdlib.h>     /* sizeof() */
#include <signal.h>     /* sig_atomic_t, sigaction(), kill() */
#include <string.h>     /* memset() */
#include <unistd.h>     /* pid_t, fork() */

/* this flag will execute the 'ping' command */
sig_atomic_t flag = 1;

void SignalHandler(int sig);

int main(int argc, char **argv)
{   int i = 0;

    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = &SignalHandler;

    sigaction(SIGUSR2, &act, NULL);


    while (1)
    {   
    	i = 0;
        sleep(2);
        
        if (flag)
        {
            printf("PING\t\t%d\n\n", (int)getppid());
            flag = 0;
            kill(getppid(), SIGUSR1);
        }

        for (i = 0; i < argc; ++i)
        {
            printf("%s\n", argv[i]);
        }

    }

    return (0);
}

void SignalHandler(int sig)
{
    (void)sig;
    flag = 1;
}
