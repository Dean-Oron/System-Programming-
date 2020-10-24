/******************************************************************************/
/* 					Variable sized memory Allocator				  			  */
/*					Description -  Using memory pool to manage fixed memory	  */
/*					Autor    -     Dean Oron								  */
/*					Date     - 	   24.03.20									  */
/*					Reviewer - 	   Samir									  */
/*					Open Lab 86											  	  */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "variable_memory_allocator.h"

#define SEG_SIZE 200

int main()
{
    void *memory = NULL;
	vsa_t *pool = NULL;
    int *block1;
    int *block2;
    int *block3;
    int *block4;
    size_t seg_size = SEG_SIZE;

    memory = malloc(seg_size);
	
    pool = VSAInit(memory, seg_size);
    
    block1 = VSAAlloc(pool, 8);

 
    printf("the largest block available after allocating forth is:%ld\n"
											, VSALargestBlockAvailable(pool));
	block2 = VSAAlloc(pool, 8);

	printf("the largest block available after allocating forth is:%ld\n"
											, VSALargestBlockAvailable(pool));
	block3 = VSAAlloc(pool, 8);

	printf("the largest block available after allocating forth is:%ld\n"
											, VSALargestBlockAvailable(pool));
	block4 = VSAAlloc(pool, 8);
	
	printf("the largest block available after allocating forth is:%ld\n"
											, VSALargestBlockAvailable(pool));
	
	*block1 = 123;
	*block2 = 456;
	*block3 = 789;
	*block4 = 777;

    VSAFree(block1);


    return(0);
}


