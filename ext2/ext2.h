/******************************************************************************/
/* 					ext2 file system implementation 		 				  */
/*					Description -   Header File		                    	  */
/*					Author    -     Dean Oron								  */
/*					Date     - 	  	05.07.20		                		  */
/*					Reviewer - 	   	Alistair Hudson					          */
/*					Open Lab 86											  	  */
/******************************************************************************/

#include <ext2fs/ext2_fs.h>

#define BASE_OFFSET 1024
#define RAM_DRIVE "/dev/ram0"
#define BLOCK_OFFSET(block) (BASE_OFFSET + (block - 1) * block_size)
#define DEFAULT_BLOCK_SIZE 1024 

unsigned int block_size;

//A function to assign the super data block of a file system to a variable
void FindSuperBlock(int fd, struct ext2_super_block *super_block);

//A function to assign the group block
void FindGroupBlock(int fd, struct ext2_group_desc *group);

//A function to read a certain inode data to a variable
void ReadInode(int fd, int inode_no,
			   const struct ext2_group_desc *group, struct ext2_inode *inode);

int FindFileInDir(int fd, const struct ext2_group_desc *group_descriptor,
				  const struct ext2_inode *inode, const char *file_name_to_search,
				  struct ext2_inode *ret_inode);

void PrintFile(int fd, struct ext2_inode *inode, int level);

void FindByPath(int fd, struct ext2_group_desc *group, char *pathname);