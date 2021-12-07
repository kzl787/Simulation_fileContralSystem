#include"structure.h"
#include<stdio.h>
#include<stdlib.h>

/*
 *功能:分配数据块空间
 *
 *算法步骤:1.检查剩余空闲块是否满足分配，满足则转2，不满足则转4；
 *        2.检查是否存在满足条件的连续的数块，满足则转3，否则装4
 *        3.将连续的数据块标记为已被使用，修改数据块位图、超级块、返回分配的首地址
 *        4.返回-1
 *参数说明：size:文件需要的盘块数    BlockAddr_index：文件存放起始盘块号
 */
long blocks_allocation(int size, int& BlockAddr_index);

/*
 功能：分配结点，返回结点号
*/
int inode_allocation();