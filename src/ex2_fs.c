#include <stdio.h>
#include <linux/fs.h>
#include </usr/include/ext2fs/ext2_fs.h> 
#include <linux/types.h> 
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>  /* lseek() */
#include <unistd.h>     /* lseek() */

#include "ext2_fs_our.h"


#define BOOT_BLOCK_SIZE (1024) /* location of super_block */
#define BLOCK_OFFSET(block) (BOOT_BLOCK_SIZE + (block - 1) * 1024)

void GetSuperBlock(int fd, struct ext2_super_block *super_block)
{
    lseek(fd, BOOT_BLOCK_SIZE, SEEK_SET);
    read(fd, super_block, sizeof(struct ext2_super_block));

    if (super_block->s_magic != EXT2_SUPER_MAGIC)
    {
        perror("Didn't reach to the super block\n");
        exit(1);
    }
}

void FindFirstGroupDecriptor(int fd, struct ext2_group_desc* ret_group)
{
    lseek(fd, BOOT_BLOCK_SIZE + sizeof(struct ext2_super_block), SEEK_SET);
    read(fd, ret_group, sizeof(struct ext2_group_desc));
}

void read_inode(int fd, 
                const struct ext2_group_desc *group_descriptor,
                int inode_number, 
                struct ext2_inode  *ret_inode)
{
    lseek(fd, BLOCK_OFFSET(group_descriptor->bg_inode_table) + (inode_number - 1) * sizeof(struct ext2_inode), SEEK_SET);
    read(fd, ret_inode, sizeof(struct ext2_inode));
}

void search_directory_by_path(int fd, 
                            const struct ext2_group_desc *group_descriptor, 
                            const struct ext2_inode *inode,
                            const char *path,
                            struct ext2_inode *ret_inode);

