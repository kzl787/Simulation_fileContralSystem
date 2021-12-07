
#include"allocation.h"

long blocks_allocation(int size, int& BlockAddr_index)
{
    long addr = -1;
    if ((int)_super_block.s_free_blocks_count < size)
        return -1;
    else
    {
        int count = 0;    //用于计算连续的空闲块
        int i;      //循环变量
        for (i = 0; i < blocks_num; i++)
        {
            if (block_bitmap[i] == not_use) {
                for (count = 0; count < size; count++, i++)     //检查后面连续空闲块是否满足要求
                    if (!(block_bitmap[i] == not_use))break;
                if (count == size) {
                    BlockAddr_index = i - size;
                    break;
                }
            }
        }
        if (i == blocks_num) return -1;                 //无满足条件的连续空闲空间
        for (int i = BlockAddr_index; i < BlockAddr_index + size; i++) //修改块图
            block_bitmap[i] = used;
        _super_block.s_free_blocks_count -= (unsigned int)size;             //修改超级快
        addr = _super_block.s_first_data_addr + BlockAddr_index * block_size;
        return addr;
    }
}


int inode_allocation()
{
    int inode_index = -1;             //记录分配的结点号
    int i;
    if (_super_block.s_free_inodes_count <= 0)return -1;
    for (i = 0; i < inodes_num; i++)
        if (inode_bitmap[i] == not_use) {
            inode_index = i;
            break;
        }
    if (i == inodes_num)return -1;
    else {
        _super_block.s_free_inodes_count -= 1;
        inode_bitmap[inode_index] = used;
    }
    return inode_index;
}