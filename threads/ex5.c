/******************************************************************************/
/* 					100k Threads						  					  */
/*					Description -  Handling threads for the first time		  */
/*					Author    -     Dean Oron								  */
/*					Date     - 	   02.05.20									  */
/*					Reviewer - 	   Amir SAraf								  */
/*					Open Lab 86											  	  */
/******************************************************************************/
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 
#include <time.h>
#include <ulimit.h>

#define NUM (100000000)

void *SumOfDivisors(void *index);

int main()
{
    size_t i = 0;
    
    size_t sum_of_divisors = 0;

    for (i = 1; i < NUM + 1; ++i)
    {
        if (0 == NUM % i)
        {
            sum_of_divisors += i;
        }
    }

        printf("the sum is %ld\n", sum_of_divisors);


    return (0);
}
