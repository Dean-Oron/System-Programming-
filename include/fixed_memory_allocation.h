/******************************************************************************/
/* 					Fixed Memory Allocator				  					  */
/*					Description -  Using memory pool to manage fixed memory	  */
/*					Autor    -     Dean Oron								  */
/*					Date     - 	   22.03.20									  */
/*					Reviewer - 	   Ben									      */
/*					Open Lab 86											  	  */
/******************************************************************************/
#ifndef FIXED_SIZE_ALLOCATION_H
#define FIXED_SIZE_ALLOCATION_H

#include <stddef.h>

typedef struct fixed_size_allocation fsa_t;

typedef struct block_header block_t;

/* A function to calculate the size necessary to allocate enough memory for
 * memory menagement with word alligned memory blocks
 * Arguments: amount of 'num_blocks' and each block size 
 * Return: the total neccessary amount of memory in bytes
 * Time Complexity: O(1) 
*/
size_t FSASuggestSize(size_t num_blocks, size_t block_size);

/*
 * Arguments:
 			 * pointer to memory allocated by the user
 			 * the total suggested size
 			 * the original block size asked by the user
 * Return: A pointer to the fixed memory allocator struct
 * Time Complexity: O(n) 
*/
fsa_t *FSAInit(void *memory, size_t seg_size, size_t block_size);

/*
 * Arguments: the fixed memory allocator
 * Return: A pointer to begining of block for data insertion by the user
 * Time Complexity: O(1) 
*/
void *FSAAlloc(fsa_t *fsa_pool);

/* A function to free a certain block for further adjustment by the user
 * Arguments: Block pointer to be freed 
 * Return: void
 * Time Complexity: O(1) 
*/
void FSAFree(void *block_ptr);

/* A function to get the number of freed block within the FSA domain in memory
 * Arguments: A pointer to the FSA DS 
 * Return: Number of free blocks to be used
 * Time Complexity: O(n) 
*/
size_t FSACountFree(const fsa_t *fsa_pool);

#endif /* FSA */
