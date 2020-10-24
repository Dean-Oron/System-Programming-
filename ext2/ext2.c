/******************************************************************************/
/* 					ext2 file system implementation 		 				  */
/*					Description -   Function Defenition                    	  */
/*					Author    -     Dean Oron								  */
/*					Date     - 	  	05.07.20		                		  */
/*					Reviewer - 	   	Alistair Hudson					          */
/*					Open Lab 86											  	  */
/******************************************************************************/
#include <sys/stat.h> //S_ISDIR
#include <stdio.h> //printf
#include <stdlib.h> //malloc
#include <unistd.h> //lseek
#include <string.h> //memcpy, strcpy

#include "ext2.h"

void FindSuperBlock(int fd, struct ext2_super_block *super)
{
	lseek(fd, BASE_OFFSET, SEEK_SET);
	read(fd, super, sizeof(*super));

	if (super->s_magic != EXT2_SUPER_MAGIC)
	{
		fprintf(stderr, "Not a Ext2 filesystem\n");
		exit(1);
	}
} 

void FindGroupBlock(int fd, struct ext2_group_desc *group)
{
	lseek(fd, BLOCK_OFFSET(2), SEEK_SET);
	read(fd, group, sizeof(struct ext2_group_desc));
} 

void ReadInode(int fd, int inode_no, const struct ext2_group_desc *group,
			   struct ext2_inode *inode)
{
	lseek(fd, BLOCK_OFFSET(group->bg_inode_table) + (inode_no - 1) * sizeof(struct ext2_inode), SEEK_SET);
	read(fd, inode, sizeof(struct ext2_inode));
} 

int FindFileInDir(int fd, const struct ext2_group_desc *group_descriptor,
				  const struct ext2_inode *inode, const char *to_find,
				  struct ext2_inode *ret_inode)
{
	void *block;
	
	if (S_ISDIR(inode->i_mode))
	{
		struct ext2_dir_entry_2 *entry;
		unsigned int size = 0;
		char file_name[EXT2_NAME_LEN + 1];

		if ((block = malloc(block_size)) == NULL)
		{
			fprintf(stderr, "Memory error\n");
			close(fd);
			exit(1);
		}

		for (int i = 0; i < inode->i_blocks; i++)
		{
			lseek(fd, BLOCK_OFFSET(inode->i_block[i]), SEEK_SET);
			read(fd, block, block_size);

			entry = (struct ext2_dir_entry_2 *)block;

			while ((size < inode->i_size) && entry->inode)
			{
				memcpy(file_name, entry->name, entry->name_len);
				file_name[entry->name_len] = '\0';
				printf("file is: %s, inode: %d\n", file_name, entry->inode);

				if (strcmp(file_name, to_find) == 0)
				{
					ReadInode(fd, entry->inode, group_descriptor, ret_inode);
					free(block);

					return (entry->inode);
				}
				entry = (void *)entry + entry->rec_len;
				size += entry->rec_len;
			}
		}

		free(block);
		block = NULL;
	}

	return (0);
}

void PrintFile(int fd, struct ext2_inode *inode, int level)
{
	char buf[DEFAULT_BLOCK_SIZE + 1] = {'\0'};
	int int_buf[DEFAULT_BLOCK_SIZE / sizeof(int) + 1] = {0};
	int i = 0;

	for (i = 0; inode->i_block[i] != 0 && i < 12; ++i)
	{
		bzero(buf, DEFAULT_BLOCK_SIZE);
		lseek(fd, inode->i_block[i] * DEFAULT_BLOCK_SIZE, SEEK_SET);
		read(fd, buf, DEFAULT_BLOCK_SIZE);
		printf("%s", buf);
	}

	if (inode->i_block[EXT2_NDIR_BLOCKS] != 0) 
	{
		lseek(fd, inode->i_block[EXT2_NDIR_BLOCKS] * DEFAULT_BLOCK_SIZE, SEEK_SET);
		read(fd, int_buf, DEFAULT_BLOCK_SIZE);

		for (i = 0; i < DEFAULT_BLOCK_SIZE / sizeof(int); ++i)
		{
			bzero(buf, DEFAULT_BLOCK_SIZE);
			lseek(fd, int_buf[i] * DEFAULT_BLOCK_SIZE, SEEK_SET);
			read(fd, buf, DEFAULT_BLOCK_SIZE);
			printf("%s", buf);
		}
	}

	if (inode->i_block[13] != 0) 
	{
		printf("\nyou exceeded the first %d blocks\n", i);
	}

	if (inode->i_block[14] != 0) /* fetch 3 level of indirect blocks */
	{
		printf("\nyou exceeded the first %d blocks\n", i);
	}

/* 	Explanation:

     i_block[0..11] point directly to the first 12 data blocks of the file.
     i_block[12] points to a single indirect block
     i_block[13] points to a double indirect block
     i_block[14] points to a triple indirect block */

}

void FindByPath(int fd, struct ext2_group_desc *group, char *pathname)
{
	struct ext2_inode inode;
	struct ext2_inode f_inode;
	char *token;
	int found = 0;

	ReadInode(fd, 2, group, &inode);

	token = strtok(pathname, "/");

	while (S_ISDIR(inode.i_mode) && token != NULL)
	{
		found = FindFileInDir(fd, group, &inode, token, &f_inode);
		printf("found:/%d token: %s\n", found, token);
		token = strtok(NULL, "/");
		inode = f_inode;
		
	}

	if (found)
	{
		printf("File was found!\nIt's content is: \n");
		PrintFile(fd, &inode, 0);
	}
	else
	{
		printf("File Not Found!\n");
	}
}