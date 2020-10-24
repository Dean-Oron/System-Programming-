/******************************************************************************/
/* 					Producer-Consumer Problem				  				  */
/*					Description -   Excercise 4                          	  */
/*					Author    -     Dean Oron								  */
/*					Date     - 	  	07.05.20		                		  */
/*					Reviewer - 	   	Noffar Gil						          */
/*					Open Lab 86											  	  */
/******************************************************************************/
#include <stdio.h> /* printf */
#include <pthread.h> /* pthread_create & join */
#include <unistd.h> /* sleep */
#include <stdatomic.h> /* __sync_lock_test_and_set & release */
#include <semaphore.h> /* sem_init, sem_wait, sem_post */
#include <fcntl.h>      /* O_CREAT */

#include "queue.h"
#include "uid.h"

/* Switch to review the different excercises */


#define THREAD_AMOUNT (3)
#define QUEUE_SIZE (20)

/* Thread function decleration */
void *Producer(void *arg);
void *Consumer(void *arg);
int global_var = 7;

int data[THREAD_AMOUNT] = {10, 20, 30};
queue_t *my_queue = NULL;
pthread_mutex_t key;
sem_t sem_in;
sem_t sem_out;

int main()
{
    pthread_t producer_arr[THREAD_AMOUNT];
    pthread_t consumer_arr[THREAD_AMOUNT];
    my_queue = QueueCreate();
    int i = 0;
    pid_t main_pid;

    pthread_mutex_init(&key, NULL);
    sem_init(&sem_in, O_CREAT, QUEUE_SIZE);
    sem_init(&sem_out, O_CREAT, 0);

	main_pid = getpid();

    /* Create producers&consumers threads */
    for (i = 0; i < THREAD_AMOUNT; ++i)
    {
        pthread_create(&producer_arr[i], NULL, &Producer, &data[i]);
        pthread_create(&consumer_arr[i], NULL, &Consumer, &data[i]);
    }

    for (i = 0; i < THREAD_AMOUNT; ++i)
    {
        pthread_join(producer_arr[i], NULL);
        pthread_join(consumer_arr[i], NULL);
    }

    QueueDestroy(my_queue);
    sem_destroy(&sem_in);
    sem_destroy(&sem_out);

	printf("main_pid is %d\n", main_pid);
    printf("global variable address - %ld\n", (size_t)&global_var);
    printf("main stack address - %ld\n", (size_t)&i);
    return (0);
}

void *Producer(void *arg)
{   
    static int index = 0;
	Uid_t my_uid;
	static int i = 0;
    int producer_stack;
    
    printf("thread stack address - %ld\n", (size_t)&producer_stack);
    while (++i < 10)
    {
        pthread_mutex_lock(&key);
        my_uid = UIDCreate();
        printf("pid %d\n", my_uid.pid);	
		printf("time %ld\n", my_uid.time);	
		printf("count %d\n\n", my_uid.SN);	
        pthread_mutex_unlock(&key);
    }
    
    return (NULL);
}

void *Consumer(void *arg)
{	
	int i = 0;
	
    while(++i < 10)
    {
        pthread_mutex_lock(&key);
		pthread_mutex_unlock(&key);
    }    
}
