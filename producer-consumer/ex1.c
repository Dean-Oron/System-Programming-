/******************************************************************************/
/* 					Producer-Consumer Problem				  				  */
/*					Description -  Excercise 1                          	  */
/*					Author    -    Dean Oron								  */
/*					Date     - 	   05.05.20									  */
/*					Reviewer - 	   Noffar Gil							      */
/*					Open Lab 86											  	  */
/******************************************************************************/
#include <stdio.h> /* printf */
#include <pthread.h> /* pthread_create & join */
#include <unistd.h> /* sleep */
#include <stdatomic.h> /* __sync_lock_test_and_set & release */

/* Switch to review the different excercises */
#define EX_A  
/* #define EX_B  */
/* #define EX_C  */
/* #define EX_D  */

#define SIZE (1000)

void *Producer(void *arg);
void *Consumer(void *arg);

int global_arr[SIZE];

#ifdef EX_B
int is_busy = 0;
#endif /* EX_B */

#ifdef EX_C
atomic_int is_busy = 0;
#endif

#ifdef EX_D
atomic_int ready_for_producer = 1;
atomic_int ready_for_consumer = 0;
#endif

int main()
{
    pthread_t producer;
    pthread_t consumer;
    
    pthread_create(&consumer, NULL, Consumer, NULL);
    pthread_create(&producer, NULL, Producer, NULL);
    
    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);

    return (0);
}

#ifdef EX_A
void *Producer(void *arg)
{   
    int i = 0;
    (void)arg;

    while (1)
    {
        for (i = 0; i < SIZE; ++i)
        {
             ++global_arr[i];
        }
    }

    return(NULL);
}

void *Consumer(void *arg)
{
    int i = 0;
    size_t sum = 0;
    (void)arg;
    
    while(1)
    {   
       sum = 0;

        for (i = 0; i < SIZE; ++i)
        {
            sum += global_arr[i];
        }

        printf("the sum is %ld\n", sum);
    }

    return(NULL);

}

#endif /* EX_A */

#ifdef EX_B
void *Producer(void *arg)
{
    int i = 0;
    (void)arg;

    while (1)
    {
        if(is_busy == 0)
        {   
            is_busy = 1;

            for (i = 0; i < SIZE; ++i)
            {
                ++global_arr[i];
            }

            is_busy = 0;
        }
    }

    return(NULL);

}

void *Consumer(void *arg)
{
    int i = 0;
    size_t sum = 0;
    (void)arg;

    while(1)
    {   
        sum = 0;
        if(is_busy == 0)
        {
            is_busy = 1;
            
            for (i = 0; i < SIZE; ++i)
            {
                sum += global_arr[i];
            }

            printf("the sum is %ld\n", sum);

            is_busy = 0;
        }
    }

    return(NULL);

}

#endif /* EX_B */

#ifdef EX_C
void *Producer(void *arg)
{
    int i = 0;
    (void)arg;

    while (1)
    {
        if (0 == is_busy)
        {
            __sync_lock_test_and_set(&is_busy, 1);
            for (i = 0; i < SIZE; ++i)
            {
                 ++global_arr[i];
            }   
            __sync_lock_release(&is_busy);
        }
    }

    return(NULL);

}

void *Consumer(void *arg)
{
    int i = 0;
    size_t sum = 0;
    (void)arg;

    while(1)
    {   
        sum = 0;

        if (0 == is_busy)
        {
            __sync_lock_test_and_set(&is_busy, 1);

            for (i = 0; i < SIZE; ++i)
            {
                sum += global_arr[i];
            }

            printf("the sum is %ld\n", sum);
            __sync_lock_release(&is_busy);
        }
    }

    return(NULL);

}

#endif /* EX_C */

#ifdef EX_D
void *Producer(void *arg)
{
    int i = 0;
    (void)arg;

    while (1)
    {
        if (0 == ready_for_producer)
        {
            for (i = 0; i < SIZE; ++i)
            {
                 ++global_arr[i];
            }   
            sleep(1);
            __sync_lock_test_and_set(&ready_for_producer, 1);
            __sync_lock_release(&ready_for_consumer);
        }
    }

    return(NULL);

}

void *Consumer(void *arg)
{
    int i = 0;
    size_t sum = 0;
    (void)arg;

    while(1)
    {   
        sum = 0;

        if (0 == ready_for_consumer)
        {
            for (i = 0; i < SIZE; ++i)
            {
                sum += global_arr[i];
            }

            printf("the sum is %ld\n", sum);
            __sync_lock_test_and_set(&ready_for_consumer, 1);
            __sync_lock_release(&ready_for_producer);
        }
    }

    return(NULL);

}

#endif /* EX_D */