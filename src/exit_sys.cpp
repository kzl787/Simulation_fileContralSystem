#define _CRT_SECURE_NO_WARNINGS
#include"exit_sys.h"

void exit_sys()
{
	errno_t err = fopen_s(&fd, "file_control_system", "rb+");
	if (fd == NULL)
	{
		strcat(print_data, "exit system error!\n");
		exit(0);
	}
	fwrite(&_super_block, superblock_bytes, 1, fd);
	fwrite(&block_bitmap, sizeof(bool), blocks_num, fd);
	fwrite(&inode_bitmap, sizeof(bool), inodes_num, fd);
	fwrite(&_inode_table, inode_bytes, inodes_num, fd);
	fseek(fd, _inode_table[current_dir.inode].i_addr, SEEK_SET);
	fclose(fd);
	exit(1);
}