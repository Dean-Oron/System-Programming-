/******************************************************************************/
/* 					PingPong Between Processes			  					  */
/*					Description -  Using signals for processes communication  */
/*								   Between two seperate processes			  */
/*					Autor    -     Dean Oron								  */
/*					Date     - 	   30.04.20									  */
/*					Reviewer - 	   Ben Zikri							      */
/*					Open Lab 86											  	  */
/******************************************************************************/
#include <signal.h> /* siginfo(), SIGUSR, sigaction(), sig_atomic_t */
#include <stdio.h>	/* printf */
#include <string.h> /* memset() */
#include <time.h> 	/* sleep() */
#include <unistd.h> /* getpid() */

sig_atomic_t flag = 0;
sig_atomic_t caller_signal = 0;

void SignalHandler(int sig);

int main()
{
    struct sigaction act;

    memset(&act, 0, sizeof(act));
	act.sa_handler = &SignalHandler;

    sigaction(SIGUSR2, &act, NULL);

    flag = 1;

    printf("program 2 started - PID %d\n", (int)getpid());

    printf("Enter ping_pong3 PID: ");
    scanf("%d", &caller_signal);

    while (1)
    {
        if (flag)
        {
            printf("PING\t\tFirst Process pid is %d\n", caller_signal);
            flag = 0;
            kill(caller_signal, SIGUSR1);
        }
    }

    return (0);
}

void SignalHandler(int sig)
{
    (void)sig;
    flag = 1;
}