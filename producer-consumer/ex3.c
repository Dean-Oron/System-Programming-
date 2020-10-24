/******************************************************************************/
/* 					Producer-Consumer Problem				  				  */
/*					Description -   Excercise 3                          	  */
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

/* Switch to review the different excercises */

#define THREAD_AMOUNT (3)

/* Thread function decleration */
void *Producer(void *arg);
void *Consumer(void *arg);

int data[THREAD_AMOUNT] = {10, 20, 30};
queue_t *my_queue = NULL;
pthread_mutex_t key;
sem_t sem;

int main()
{
    pthread_t producer_arr[THREAD_AMOUNT];
    pthread_t consumer_arr[THREAD_AMOUNT];
    my_queue = QueueCreate();
    int i = 0;

    pthread_mutex_init(&key, NULL);
    sem_init(&sem, O_CREAT, 0);

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
    sem_destroy(&sem);

    return (0);
}

void *Producer(void *arg)
{   
    static int index = 0;

    while (1)
    {   
        sleep(1);
        pthread_mutex_lock(&key);
        QueueEnqueue(my_queue, &data[index]);
        pthread_mutex_unlock(&key);
        sem_post(&sem);
        printf("number %d was inserted\n", data[index]);
        printf("Queue size after producing is %ld\n", QueueSize(my_queue));


        if (++index == THREAD_AMOUNT)
        {
            index = 0;
        }
    }
    
    return (NULL);
}

void *Consumer(void *arg)
{
    while(1)
    {   
        printf("Queue size before consuming is %ld\n", QueueSize(my_queue));
        sem_wait(&sem); 
        pthread_mutex_lock(&key);
        QueueDequeue(my_queue);
        printf("Data was poped out from queue seccessfuly\n");
        pthread_mutex_unlock(&key);
    }

    return (NULL);    
}
