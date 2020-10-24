/******************************************************************************/
/* 					Variable sized memory Allocator				  			  */
/*					Description -  Using memory pool to manage fixed memory	  */
/*					Autor    -     Dean Oron								  */
/*					Date     - 	   24.03.20									  */
/*					Reviewer - 	   Samir									  */
/*					Open Lab 86											  	  */
/******************************************************************************/
#ifndef VARIABLE_SIZE_ALLOCATION_H
#define VARIABLE_SIZE_ALLOCATION_H

#define LAST_BLOCK 9999999999999999	

#include <stddef.h>

typedef struct block_header vsa_t;

typedef struct block_header block_t;

/* Faunction to initilize new varable size memory segment for memory manegment
 * Arguments:
 			 * Pointer to begining of memory segment allocated by the user
 			 * Total memory segment size in bytes 
 * Return: A pointer to the first user data block
 * Time complexity O(1)
*/
block_t *VSAInit(void *memory, size_t seg_size);

/* A function to allocate a costumized size memory block
 * Arguments: 
 			 * A pointer to the begining of memory segment vsa_pool
 			 * The size of block to be allocated 'block_size'
 * Return: A pointer to the allocated user data block
 * Time complexity O(n)
*/
void *VSAAlloc(vsa_t *vsa_pool, size_t block_size);

/* A function to free the block memory pointed by block_ptr
 * Arguments: Free the block pointed by 'block_ptr' within memory segment
 			 * A pointer to the required block to be freed
 			 * The block_size will be increased to word alligned value if need
 * Return: void
 * Time complexity O(1)
*/
void VSAFree(void *block_ptr);

/* A function to get the largest memory block in the memory segment
 * Arguments: 
 			 * A pointer to the variable size memory allocator DS  
 * Return: The size of the largest block available within memory segment
 * Time complexity: O(n)
*/
size_t VSALargestBlockAvailable(vsa_t *pool);

#endif /* VSA */
