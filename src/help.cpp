#define _CRT_SECURE_NO_WARNINGS
#include"help.h"
/*
 功能：打印命令格式
*/
void help()
{
	strcat(print_data, "\n以下虚线左边为功能,右边为命令格式,输入时请按格式输入!\n");
	strcat(print_data, "1.路径支持绝对路径和相对路径,当前目录为 . 上一级目录为 .. \n");
	strcat(print_data, "3.当与host文件系统间进行文件拷贝时路径名前应加<host>,且路径间用\\隔开\n\n");
	strcat(print_data, "0.初始化---------------format\n");
	strcat(print_data, "1.显示系统信息---------info\n");
	strcat(print_data, "2.改变目录-------------cd + 空格 + 路径\n");
	strcat(print_data, "3.显示目录-------------dir + 空格 + 路径\n");
	strcat(print_data, "4.创建目录-------------md + 空格 + 路径(包含目录名)\n");
	strcat(print_data, "5.删除目录-------------rd + 空格 + 路径(包含目录名)\n");
	strcat(print_data, "6.建立文件-------------newfile + 空格 + 路径(包含文件名)\n");
	strcat(print_data, "7.打开文件-------------cat + 空格 + 路径(包含文件名)\n");
	strcat(print_data, "8.拷贝文件-------------copy + 空格 + 源路径 + 空格 + 目标路径\n");
	strcat(print_data, "9.删除文件-------------del + 空格 +路径(包含文件名)\n");
	strcat(print_data, "10.检测并恢复文件系统--check\n");
	strcat(print_data, "11.退出文件模拟--------exit\n");
	strcat(print_data, "12.帮助----------------help\n\n");
}