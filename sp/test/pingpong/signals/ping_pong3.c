/******************************************************************************/
/* 					PingPong Between Processes			  					  */
/*					Description -  Using signals for processes communication  */
/*								   Between two seperate processes			  */
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

sig_atomic_t flag1 = 0;
sig_atomic_t flag2 = 1;
sig_atomic_t caller_signal = 0;

void SpeacialSigHandler(int sig, siginfo_t *info, void *ucontext);

int main()
{
	struct sigaction act;

	memset(&act, 0, sizeof(act));
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = &SpeacialSigHandler;

	sigaction(SIGUSR1, &act, NULL);

	printf("The pid of this process is %d\n\n", (int)getpid());

	while (1)
	{	
		sleep(1);
	
		if (flag1)
		{
			printf("PONG! Saperate Process pid is %d\n", caller_signal);
			flag1 = 0;
			kill(caller_signal, SIGUSR2);
		}
	}
	return (0);
}

void SpeacialSigHandler(int sig, siginfo_t *info, void *vp)
{
	(void)sig;
	flag1 = 1;
	caller_signal = info->si_pid;
}