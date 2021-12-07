#define _CRT_SECURE_NO_WARNINGS
#include"check.h"

/*
 功能：检查系统
 实现步骤：检查超级块中记录的空闲块数和空闲结点与数据块位图和结点位图的的统计结果是否相同；
         不相同则修改超级快描述符；
*/
void check()
{
    unsigned int free_b = 0, free_i = 0;
    for (int i = 0; i < blocks_num; i++)
    {
        if (block_bitmap[i] == not_use)free_b++;
        if (inode_bitmap[i] == not_use)free_i++;
    }
    bool change = false;
    if (_super_block.s_free_blocks_count != free_b)
    {
        _super_block.s_free_blocks_count = free_b;
        change = true;
    }
    if (_super_block.s_free_inodes_count != free_i)
    {
        _super_block.s_free_inodes_count = free_i;
        change = true;
    }
    if (change) {
        strcat(print_data, "系统异常， 正在修复中....\n");
        errno_t err = fopen_s(&fd, "file_control_system", "rb+");
        if (err != 0)
        {
            strcat(print_data, "file_control_system open fail!\n");
            exit(0);
        }
        fseek(fd, 0, SEEK_SET);
        fwrite(&_super_block, superblock_bytes, 1, fd);
        fwrite(&block_bitmap, sizeof(bool), blocks_num, fd);
        fwrite(&inode_bitmap, sizeof(bool), inodes_num, fd);
        fclose(fd);
    }
    else
    {
        strcat(print_data, "系统处于正常状态！\n");
    }
}