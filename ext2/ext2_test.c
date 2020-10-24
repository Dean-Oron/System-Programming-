/******************************************************************************/
/* 					ext2 file system implementation 		 				  */
/*					Description -   Test File 	                         	  */
/*					Author    -     Dean Oron								  */
/*					Date     - 	  	05.07.20		                		  */
/*					Reviewer - 	   	Alistair Hudson					          */
/*					Open Lab 86											  	  */
/******************************************************************************/
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "ext2.h"

unsigned int block_size = 0;

int main(void)
{
	struct ext2_super_block super_block;
	struct ext2_group_desc group;
	struct ext2_inode inode;
	struct ext2_inode inode2;
	struct ext2_inode f_inode;
	int fd;
	int found = 0;
	char c = 'n';

	// Will locate this file and show what happen after the 12 first pointed block
	char pathname[] = "valgrind.h";

	if ((fd = open(RAM_DRIVE, O_RDONLY)) == -1)
	{
		perror(RAM_DRIVE);
		return (1); 
	}

	FindSuperBlock(fd, &super_block);
	block_size = 1024 << super_block.s_log_block_size;

	ReadInode(fd, 2, &group, &inode); 
	FindGroupBlock(fd, &group);
	FindByPath(fd, &group, pathname);
	close(fd);

	return (0);
} /* main() */
