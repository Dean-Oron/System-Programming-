/******************************************************************************/
/* 					Fixed Memory Allocator				  					  */
/*					Description -  Using memory pool to manage fixed memory	  */
/*					Author    -     Dean Oron								  */
/*					Date     - 	   22.03.20									  */
/*					Reviewer - 	   Ben									      */
/*					Open Lab 86											  	  */
/******************************************************************************/
#include <stdio.h>
#include <assert.h>

#include "fixed_memory_allocation.h"

#define WORD_SIZE sizeof(void *)

static void SwapOffset(size_t *offset1, size_t *offset2);

struct fixed_size_allocation
{
      size_t block_size;
      size_t num_blocks;
      size_t next_free;
};

struct block_header
{
        size_t next_free;  /* Offset (in bytes) from the start of the FSA. */
};

size_t FSASuggestSize(size_t num_blocks, size_t block_size)
{
	size_t total = 0;
	size_t reminder = 0;

	assert(num_blocks && block_size);
	
	reminder = block_size % WORD_SIZE; 

	if (0 != reminder)
	{
		block_size = block_size + WORD_SIZE - reminder; 
	}
	
	total = (sizeof(block_t) + block_size) * num_blocks + sizeof(fsa_t);

	return (total);
}

fsa_t *FSAInit(void *memory, size_t seg_size, size_t block_size)
{
	size_t reminder = 0;
	size_t blocks = 0;
	size_t offset = 0;
	fsa_t *root = (fsa_t *)memory;
	block_t *block_ptr = NULL;
	char *offset_ptr = NULL;
	
	root->next_free = sizeof(fsa_t);
	reminder = block_size % WORD_SIZE;
	
	if (reminder != 0)
	{
		block_size = block_size + (WORD_SIZE - reminder);
	}
	
	root->block_size = block_size;
	
	for (offset = root->next_free; offset < seg_size; ++blocks, 
							offset = offset + block_size + sizeof(block_t *))
	{	
		offset_ptr = (char *)root;
		offset_ptr += offset;
		block_ptr = (block_t *)offset_ptr;
		
		if (offset == seg_size - block_size - sizeof(block_t *))
		{
			block_ptr->next_free = 0;
		}
		
		else
		{
			block_ptr->next_free = offset + sizeof(block_t) + block_size;
		}	 
		
		printf("block_ptr->next_free is %ld\n", block_ptr->next_free);
	}
	
	root->num_blocks = blocks;
	
	printf("block size was increased for alignment matters to %ld\n", block_size);
	return (root);
}

void *FSAAlloc(fsa_t *fsa_pool)
{
	char *to_return = NULL;
	block_t *block_ptr = NULL;
	
	if (fsa_pool->next_free == 0)
	{
		printf("No more free memory, to insert data, free block first\n");
		
		return (NULL);
	}
	
	to_return = (char *)fsa_pool + fsa_pool->next_free;
	block_ptr = (block_t *)to_return;
	SwapOffset(&fsa_pool->next_free, &block_ptr->next_free);

	return (to_return + sizeof(block_t));
}

void FSAFree(void *block_ptr)
{
	fsa_t* pool = NULL;
	block_t *to_free = NULL;
	char *root = NULL;
	char *offset = (char *)block_ptr - sizeof(block_t);
	
	to_free = (block_t *)offset;
	
	root = (char *)to_free - to_free->next_free;
	
	pool = (fsa_t *)root;
	
	SwapOffset(&pool->next_free, &to_free->next_free);
}

size_t FSACountFree(const fsa_t *fsa_pool)
{
	size_t count_free = 0;
	size_t offset = 0;
	block_t *block_ptr = NULL;
	char *offset_ptr = NULL;
	
	offset = fsa_pool->next_free;
	
	while (offset != 0)
	{
		offset_ptr = (char *)fsa_pool;
		offset_ptr += offset;
		block_ptr = (block_t *)offset_ptr;
		offset = block_ptr->next_free;
		
		++count_free;
	}
	
	return (count_free);
}

/*******************************Utility Functions******************************/

static void SwapOffset(size_t *offset1, size_t *offset2)
{
	size_t tmp = *offset1;
	*offset1 = *offset2;
	*offset2 = tmp;
}
