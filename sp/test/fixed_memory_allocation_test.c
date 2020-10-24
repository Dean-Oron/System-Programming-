/******************************************************************************/
/* 					Fixed Memory Allocator				  					  */
/*					Description -  Using memory pool to manage fixed memory	  */
/*					Autor    -     Dean Oron								  */
/*					Date     - 	   22.03.20									  */
/*					Reviewer - 	   Ben									      */
/*					Open Lab 86											  	  */
/******************************************************************************/
#include <stdio.h>	/* printf() */
#include <stdlib.h> /* malloc(), free() */

#include "fixed_memory_allocation.h"

int main()
{
	size_t num_blocks = 4;
	size_t block_size = 45;
	size_t total_size = 0;
	block_t *block_ptr = NULL;
	char *offset = NULL;
	fsa_t *my_fsa = NULL;
	char *block1;
	char *block2;
	char *block3;
	int *block4;
	void *memory = NULL;

	total_size = FSASuggestSize(num_blocks, block_size);
	
	printf("total size %ld\n\n", total_size);
	
	memory = (void *)malloc(total_size);
	
	my_fsa = FSAInit(memory, total_size, block_size);
	
	block1 = FSAAlloc(my_fsa);
	block2 = FSAAlloc(my_fsa);
	block3 = FSAAlloc(my_fsa);
	block4 = FSAAlloc(my_fsa);	
	
	block1 = "Hi Benzi whatsapp? thanks for the code review";
	block3 = "Hope you didn't get corona yet, take care my friend";
	*block4 = 777;
	
	printf("%s\n", block1);
	printf("%s\n", block3);
	printf("%d\n", *block4);
	
	FSAFree(block4);
			
	printf("the amount of free blocks is %ld\n", FSACountFree(my_fsa));
	
	return (0);
}




