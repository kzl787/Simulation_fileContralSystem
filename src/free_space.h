#include"structure.h"
#include<stdio.h>
#include<stdlib.h>
/*
*功能：释放数据块（调用前需打开系统文件， 函数中会将修改完的位图,以及超级快写回系统）
*参数说明： count：释放数据块的数量； b_index：释放的第一个数据块号
*/
bool free_blocks(int count, int b_index);

/*
 功能：释放结点；(调用前需要打开系统文件，函数会将修改完的位图和超级快写回系统)
*/
bool free_inode(int i_index);