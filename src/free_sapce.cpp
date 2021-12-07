#define _CRT_SECURE_NO_WARNINGS
#include"free_space.h"

bool free_blocks(int count, int b_index)
{
	int i = 0;
	for (i = b_index; i < b_index + count; i++)
	{
		block_bitmap[i] = not_use;
	}
	fseek(fd, superblock_bytes + sizeof(bool) * b_index, SEEK_SET);
	fwrite(&block_bitmap[b_index], sizeof(bool), count, fd);
	_super_block.s_free_blocks_count += (unsigned int)count;
	fseek(fd, 0, SEEK_SET);
	fwrite(&_super_block, superblock_bytes, 1, fd);
	return true;
}


bool free_inode(int i_index)
{
	inode_bitmap[i_index] = not_use;
	fseek(fd, superblock_bytes + sizeof(bool) * (blocks_num + i_index), SEEK_SET);
	fwrite(&inode_bitmap[i_index], sizeof(bool), 1, fd);
	_super_block.s_free_inodes_count += 1;
	fseek(fd, 0, SEEK_SET);
	fwrite(&_super_block, superblock_bytes, 1, fd);
	return true;
}