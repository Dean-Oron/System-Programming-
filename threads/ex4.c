/******************************************************************************/
/* 					100k Threads						  					  */
/*					Description -  Handling threads for the first time		  */
/*					Author    -     Dean Oron								  */
/*					Date     - 	   02.05.20									  */
/*					Reviewer - 	   Amir Saraf								  */
/*					Open Lab 86											  	  */
/******************************************************************************/

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 
#include <time.h>
#include <ulimit.h>

#define NUM (100000000)
#define NUM_OF_THREADS (5)

void *SumOfDivisors(void *index);
int step = NUM / NUM_OF_THREADS;

int main()
{
    int i = 0;
    size_t sum = 0;

    size_t x1 = 1;
    size_t x2 = 1 * NUM / 5 + 1;
    size_t x3 = 2 * NUM / 5 + 1;
    size_t x4 = 3 * NUM / 5 + 1;
    size_t x5 = 4 * NUM / 5 + 1;
    
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;        
    pthread_t thread4;
    pthread_t thread5;

    pthread_create(&thread1, NULL, SumOfDivisors, &x1);
    pthread_create(&thread2, NULL, SumOfDivisors, &x2);
    pthread_create(&thread3, NULL, SumOfDivisors, &x3);
    pthread_create(&thread4, NULL, SumOfDivisors, &x4);
    pthread_create(&thread5, NULL, SumOfDivisors, &x5);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);
    pthread_join(thread5, NULL);



    sum = x1 + x2 + x3 + x4 + x5;

    printf("the sum is %ld\n", sum);

    return (0);
}

void *SumOfDivisors(void *index) 
{   
    int i = *(int*)index;
    int *tmp = (int *)index;
    int sum_of_divisors = 0;
    int j = 0;
        
    for (j = i; j < i + step; ++j)
    {
        if (0 == NUM % j)
        {
            sum_of_divisors += j;
        }
    }

    *tmp = sum_of_divisors;

    return (NULL);
}
