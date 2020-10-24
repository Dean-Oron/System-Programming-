
#include <stdio.h> /* printf */
#include <pthread.h> /* pthread_create & join */
#include <unistd.h> /* sleep */
#include <stdatomic.h> /* __sync_lock_test_and_set & release */
#include <semaphore.h> /* sem_init, sem_wait, sem_post */
#include <fcntl.h>      /* O_CREAT */

void *Producer(void *arg);
void *Consumer(void *arg);
pthread_mutex_t key;
sem_t sem;
pthread_cond_t cond;

#define SIZE (7)
#define THREAD_AMOUNT (3)
char str[SIZE] = {0};

int main()
{
    pthread_t producer;
    pthread_t consumer_arr[THREAD_AMOUNT];
    int i = 0;
    pthread_mutex_init(&key, NULL);
    pthread_cond_init(&cond, NULL);

    sem_init(&sem, O_CREAT, 0);
    
    pthread_create(&producer, NULL, &Producer, str);

    for (i = 0; i < THREAD_AMOUNT; ++i)
    {
        pthread_create(&consumer_arr[i], NULL, &Consumer, NULL);
    }

    sem_destroy(&sem);

    return (0);
}

void *Producer(void *arg)
{
    (void)arg;
    
    pthread_mutex_lock(&key);
    str[0] = 'x';
    
    pthread_cond_wait(&cond, &key);

    pthread_mutex_unlock(&key);
    
    return (NULL);
}

void *Consumer(void *arg)
{
    int counter = 0;
    (void)arg;

    sem_getvalue(&sem, &counter);

    pthread_mutex_lock(&key);

    printf("data is %c", str[0]);
    if (counter == THREAD_AMOUNT)
    {
        pthread_cond_signal(&cond);
    }

    pthread_mutex_unlock(&key);

    sem_post(&sem);

    return (NULL);
}