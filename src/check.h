#include"structure.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*
 功能：检查系统
 实现步骤：检查超级块中记录的空闲块数和空闲结点与数据块位图和结点位图的的统计结果是否相同；
         不相同则修改超级快描述符；
*/
void check();
