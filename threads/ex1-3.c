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

#define SIZE (100000)

void *MyThread(void *index);
void PrintArr(int *arr);


int global_arr[SIZE];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int main()
{
    int i = 0;
    int status = 1;
    pthread_t thread;

    for (i = 0; i < SIZE; ++i)
    {      
        status =  pthread_create(&thread, NULL, MyThread, &i);
        
       
        while(status != 0)
        {   
            status =  pthread_create(&thread, NULL, MyThread, &i);
            printf("keep try %d\n", status);
        }

        /*pthread_join(thread, NULL);*/    
        printf("create returns %d\n", status);
    }

    sleep (1);

    PrintArr(global_arr);

    return (0);
} 

void *MyThread(void *index) 
{   
    int *i = (int *)index;

    pthread_detach(pthread_self()); /* for exercise 3 */
    printf("the index is %d \n", *i); 
    
    global_arr[*i] = *i;
    
    return NULL;
}

void PrintArr(int *arr)
{
    int i = 0;

    for(i = 0; i < SIZE; ++i)
    {   
        sleep(0.1);
        printf("%d\n", arr[i]);
    }
}
