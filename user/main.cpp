#define _CRT_SECURE_NO_WARNINGS
#include"memory.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

char* data;
int main()
{
	char* str = new char[80];
	memset(str, 0, 80);
	sprintf(str, "%s文件 %s 的内容如下:\n\n", str, "these");
	strcat(str, "strings ");
	strcat(str, "are ");
	strcat(str, "concatenated.");
	puts(str);
	return 0;

}
/*
int main()
{
	if (initShareMemory())
	{
		char wellcome[] = "默认管理员账号：341496\n密码：123456\n\n——————————欢迎使用模拟系统——————————\n\0";
		writeData(DATA_ADDR, wellcome);
		char homePath[] = "root>\0";
		writePath(CURRENT_PATH_ADDR, homePath);
		writeFlag(FLAG_ADDR, 1);
		//while (!login());
		char flag;
		bool check_exit = true;
		while (check_exit)
		{
			char order1[100], path1[1024], path2[1024];
			while (true)
			{
				readFlag(FLAG_ADDR, flag);
				if (flag == 0)break;
			}
			readData(DATA_ADDR, order1);            //读取命令
			writeFlag(FLAG_ADDR, 1);
			int i = 0;
			for (; i < 13; i++)
				if (strcmp(_order[i].oreder_name, order1) == 0)
					break;
			switch (i)                       //将路径读取进来
			{
			case 2:		//cd
			case 3:		//dir
			case 4:		//md
			case 5:		//rd
			case 6:		//newfile
			case 7:		//cat
			case 9:		//del
				while (true)
				{
					readFlag(FLAG_ADDR, flag);
					if (flag == 0)break;
				}
				readData(DATA_ADDR, path1);
				break;
			case 8:		//copy
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
			char* data = new char[100];
			memset(data, 0, 100);
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
				check_exit = false;
				break;
			case 12:	//help
				help();
				break;
			default:
				strcpy(print_data, "传输命令不正确！\n");
				break;
			}
			Sleep(1000);
			writeData(DATA_ADDR, data);
			writeFlag(FLAG_ADDR, 1);
			delete[] data;
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
*/