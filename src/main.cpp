#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"structure.h"
#include"format.h"
#include"dir_order.h"
#include"allocation.h"
#include"free_space.h"
#include"file_oreder.h"
#include"check.h"
#include"login.h"
#include"info.h"
#include"exit_sys.h"
#include"help.h"
#include"memory.h"


struct super_block _super_block;
bool block_bitmap[blocks_num];
bool inode_bitmap[inodes_num];
struct inode _inode_table[inodes_num];
struct dir current_dir;
FILE* fd;
unsigned int uid;
char current_path[500];
struct user _user[max_user_num];
struct order _order[13];
char* print_data;

int main()
{
	load();
	if (initShareMemory())
	{
		char wellcome[] = "默认管理员账号：341496\n密码：123456\n\n——————————欢迎使用模拟系统——————————\n\0";
		writeData(DATA_ADDR, wellcome);
		char homePath[] = "root>\0";
		writePath(CURRENT_PATH_ADDR, homePath);
		writeFlag(FLAG_ADDR, 1);
		//while (!login());
		char order1[100], path1[1024], path2[1024];
		bool _exit = true;
		while(_exit)
		{
			char flag;
			while (true)
			{
				readFlag(FLAG_ADDR, flag);
				if (flag == 0)break;
			}
			readData(DATA_ADDR, order1);            //读取命令
			int i = 0;
			for (; i < 13; i++)
				if (strcmp(_order[i].oreder_name, order1) == 0)
					break;
			switch (i)                       //读取路径进来
			{
			case 2:		//cd
			case 3:		//dir
			case 4:		//md
			case 5:		//rd
			case 6:		//newfile
			case 7:		//cat
			case 9:		//del
				writeFlag(FLAG_ADDR, 1);
				while (true)
				{
					readFlag(FLAG_ADDR, flag);
					if (flag == 0)break;
				}
				readData(DATA_ADDR, path1);
				break;
			case 8:		//copy
				writeFlag(FLAG_ADDR, 1);
				while (true)
				{
					readFlag(FLAG_ADDR, flag);
					if (flag == 0)break;
				}
				readData(DATA_ADDR, path1);
				writeFlag(FLAG_ADDR, 1);
				while (true)
				{
					readFlag(FLAG_ADDR, flag);
					if (flag == 0)break;
				}
				readData(DATA_ADDR, path2);
				break;
			default:
				break;
			}
			print_data = new char[MAX_BUF_SIZE];
			memset(print_data, 0, MAX_BUF_SIZE);
			switch (i)              //根据对应命令调用对应函数
			{
			case 0:		//format
				format();
				break;
			case 1:		//info
				info();
				break;
			case 2:		//cd
				change_dir(path1);
				memset(CURRENT_PATH_ADDR, 0, MAX_PATH_SIZE);
				writePath(CURRENT_PATH_ADDR, current_path);
				break;
			case 3:		//dir
				display_dir(path1);
				break;
			case 4:		//md
				creat_dir(path1);
				break;
			case 5:		//rd
				remove_dir(path1);
				break;
			case 6:		//newfile
				create_file(path1);
				break;
			case 7:		//cat
				open_file(path1);
				break;
			case 8:		//copy
				copy_file(path1, path2);
				break;
			case 9:		//del
				del_file(path1);
				break;
			case 10:	//check
				check();
				break;
			case 11:	//exit
				_exit = false;
				break;
			case 12:	//help
				help();
				break;
			default:
				strcpy(print_data, "传输命令不正确！\n\0");
				break;
			}
			flushData(DATA_ADDR);
			writeData(DATA_ADDR, print_data);
			writeFlag(FLAG_ADDR, 1);
			delete[] print_data;
		}
		if (pBuffer != NULL)
			UnmapViewOfFile(pBuffer);
		if (hMapFile != NULL)
			CloseHandle(hMapFile);
		exit_sys();
	}
	else
	{
		printf("simdisk初始化共享空间失败！\n");
	}
	return 0;
}