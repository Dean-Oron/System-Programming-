/******************************************************************************/
/* 					Variable sized memory Allocator				  			  */
/*					Description -  Using memory pool to manage fixed memory	  */
/*					Autor    -     Dean Oron								  */
/*					Date     - 	   24.03.20									  */
/*					Reviewer - 	   Samir									  */
/*					Open Lab 86											  	  */
/******************************************************************************/
#include <sys/types.h> /* ssize_t */
#include <stdio.h>
#include <stdlib.h> /* abs */
#include <assert.h>

#include "variable_memory_allocator.h"

#define WORD_SIZE sizeof(size_t)
#define VALIDATED 777777777777
#define ND
struct block_header /* move to function definitin file */
{
	ssize_t block_size;

#ifndef ND
	long valid;
#endif
};

vsa_t *VSAInit(void *memory, size_t seg_size)
{
	vsa_t *seg_start = memory;
	block_t *last = NULL;
	size_t remain = 0;

	assert (memory && seg_size);
	
	remain = seg_size % WORD_SIZE;
	seg_size -= remain;
	seg_start->block_size = seg_size - 2 * sizeof(block_t *);
	last = seg_start + (seg_start->block_size / sizeof(block_t)) + 1;
	last->block_size = LAST_BLOCK;
	
	#ifndef ND
	seg_start->valid = VALIDATED;
	last->valid = VALIDATED;
	#endif

	return (seg_start);
}

/******************************************************************************/
void *VSAAlloc(vsa_t *vsa_pool, size_t block_size)
{
	block_t *block_probe = (block_t *)vsa_pool;
	block_t *current_block = NULL;
	size_t remain = 0;

	remain = block_size % sizeof(block_t);
	
	if (remain != 0)
	{
		block_size += (sizeof(block_t) - remain);
	}
	if (block_size > VSALargestBlockAvailable(vsa_pool))
	{
		printf("Requested block_size is bigger than largest block available\n");
		
		return (NULL);
	}
	
		/* as long didnt reach to the end */		
	while(block_probe->block_size != LAST_BLOCK)
	{	
		current_block = block_probe;
		
	/* merge as long as available but not enough room */  
		while(current_block->block_size > 0 && current_block->block_size < (ssize_t)block_size)
		{
			block_probe += (block_probe->block_size / sizeof(block_t)) + 1;

			if (block_probe->block_size > 0)
			{
				current_block->block_size += block_probe->block_size + sizeof(block_t);
			}
            else
			{                
				break;
			}
		}
		
		/* if there is too much room */
		if (current_block->block_size > (ssize_t)block_size + (ssize_t)sizeof(block_t)) 
		{	
			block_probe = current_block + (block_size / sizeof(block_t)) + 1;
			
			/* Split block */
			block_probe->block_size = current_block->block_size - block_size - sizeof(block_t);
			
			#ifndef ND
				block_probe->valid = VALIDATED;
			#endif
			
			current_block->block_size = block_size * (-1);
			
			#ifndef ND
			current_block->valid = VALIDATED;
			#endif
			return (current_block + 1);
        }
        /* exactly the block size user requested */
        else if (current_block->block_size == (ssize_t)block_size)
        {	
        	current_block->block_size = block_size * (-1);
			#ifndef ND
			current_block->valid = VALIDATED;
			#endif
        	return (current_block + 1);
        }
        
		block_probe += (abs(block_probe->block_size) / sizeof(block_t)) + 1;
	}
	
	return (NULL);
}
/******************************************************************************/
void VSAFree(void *block_ptr)
{
	block_t *current_block = (block_t *)block_ptr - 1;
	
	assert (block_ptr);
	
	#ifndef ND
	if (current_block->valid != VALIDATED)
	{
		printf("Position error occur!!! (Inside free)\n");
	}
	#endif
	
	current_block->block_size *= (-1);
}
/*****************************************************************************/
size_t VSALargestBlockAvailable(vsa_t *pool)
{
    size_t largest = 0;
    block_t *block_ptr = (block_t *)pool;
	assert (pool);
	
    while (block_ptr->block_size != LAST_BLOCK)
    {
    	#ifndef ND
		if (block_ptr->valid != VALIDATED)
		{
			printf("Position error occur!!!(Inside largest)\n");
		}
		#endif
		
            if (block_ptr->block_size > (ssize_t)largest)
            {
                largest = block_ptr->block_size;
            }
            #ifndef ND
            printf("the current block address is %ld\n", (size_t)block_ptr);
			printf("the current block size is %ld\n", block_ptr->block_size);
			#endif
            block_ptr += (abs(block_ptr->block_size) / sizeof(block_t)) + 1;
    }
    
    return (largest);
}
