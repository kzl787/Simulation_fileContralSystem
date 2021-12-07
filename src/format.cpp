#define _CRT_SECURE_NO_WARNINGS
#include"format.h"


void format()
{
	strcat(print_data,"初始化系统文件中！\0");

	long system_size = first_data_addr + blocks_num * block_size;    //系统的全部大小
	char* buffer = new char[system_size];
	memset(buffer, 0, system_size);
	errno_t err = fopen_s(&fd, "file_control_system", "wb+");
	if (err != 0)
	{
		strcat(print_data, "file_control_system creat file!\n");
		exit(0);
	}
	fseek(fd, 0, SEEK_SET);
	fwrite(buffer, system_size, 1, fd);
	fclose(fd);
	delete[] buffer;

	//初始化超级块
	_super_block.s_blocks_size = block_size;
	_super_block.s_blocks_count = blocks_num;
	_super_block.s_inodes_count = inodes_num;
	_super_block.s_first_data_addr = first_data_addr;
	_super_block.s_free_blocks_count = blocks_num;
	_super_block.s_free_inodes_count = inodes_num;

	//初始化两个位图
	int i = 0;
	for (i = 0; i < inodes_num; i++)
		inode_bitmap[i] = not_use;
	for (i = 0; i < blocks_num; i++)
		block_bitmap[i] = not_use;

	//初始化结点区
	for (i = 0; i < inodes_num; i++)
	{
		_inode_table[i].i_blocks = 0;
		_inode_table[i].i_change = false;
		_inode_table[i].i_model = _file;
		_inode_table[i].i_size = 0;
		_inode_table[i].i_uid = 0;
		_inode_table[i].i_limit = read_only;
		_inode_table[i].i_addr = 0;
	}

	/*创建根目录*/
	//初始化根目录的inode
	_inode_table[0].i_blocks = dir_sizeof_block;
	_inode_table[0].i_addr = first_data_addr;
	_inode_table[0].i_change = false;
	_inode_table[0].i_model = _directory;
	_inode_table[0].i_size = sizeof(dir);
	_inode_table[0].i_uid = 341496;
	_inode_table[0].i_limit = read_only;
	//更改超级快、位图等信息
	_super_block.s_free_blocks_count -= dir_sizeof_block;
	_super_block.s_free_inodes_count -= 1;
	inode_bitmap[0] = used;
	for (i = 0; i < dir_sizeof_block; i++)
		block_bitmap[i] = used;
	//初始化根目录的信息
	strcpy(current_dir.dir_name, "root");         //目录名
	current_dir.inode = 0;                      //根目录的索引结点
	current_dir.size = 2;                       //文件数量
	strcpy(current_dir.file_entry[0].name, "..");  //父目录
	current_dir.file_entry[0].inode = 0;          //是根目录
	strcpy(current_dir.file_entry[1].name, ".");  //当前目录
	current_dir.file_entry[1].inode = 0;          //也是根目录

	//初始化命令名结构组
	strcpy(_order[0].oreder_name, "format");
	strcpy(_order[1].oreder_name, "info");
	strcpy(_order[2].oreder_name, "cd");
	strcpy(_order[3].oreder_name, "dir");
	strcpy(_order[4].oreder_name, "md");
	strcpy(_order[5].oreder_name, "rd");
	strcpy(_order[6].oreder_name, "newfile");
	strcpy(_order[7].oreder_name, "cat");
	strcpy(_order[8].oreder_name, "copy");
	strcpy(_order[9].oreder_name, "del");
	strcpy(_order[10].oreder_name, "check");
	strcpy(_order[11].oreder_name, "exit");
	strcpy(_order[12].oreder_name, "help");

	//将系统初始化信息写入虚拟磁盘
	err = fopen_s(&fd, "file_control_system", "rb+");
	if (err != 0)
	{
		strcat(print_data, "file_control_system creat failed !\n");
		exit(0);
	}
	fseek(fd, 0, SEEK_SET);
	fwrite(&_super_block, superblock_bytes, 1, fd);
	fwrite(&block_bitmap, sizeof(bool), blocks_num, fd);
	fwrite(&inode_bitmap, sizeof(bool), inodes_num, fd);
	fwrite(&_inode_table, inode_bytes, inodes_num, fd);
	fwrite(&current_dir, dir_bytes, 1, fd);
	fclose(fd);

}

void load()
{
	errno_t err = fopen_s(&fd, "file_control_system", "rb+");
	if (err != 0)
	{
		char select = 's';	//初始化为s，无意义
		fflush(stdin);		//清除流
		do
		{
			if (select != '\n')
				strcat(print_data, "模拟Linux系统文件不存在，是否进行格式化(y/n): \n");
			select = getchar();              //小小的bug，因为文件医存在，所以具体不会执行
			if (select == 'y' || select == 'Y' || select == 'n' || select == 'N')
			{
				fflush(stdin);		//清除流
				break;
			}
		} while (1);
		if (select == 'y' || select == 'Y')
		{
			format();
			return;
		}
		else
		{
			strcat(print_data, "模拟Linux文件系统打开失败!\n");
			exit(0);
		}
	}

	//读入系统信息
	fseek(fd, 0, SEEK_SET);
	fread(&_super_block, superblock_bytes, 1, fd);
	fread(&block_bitmap, sizeof(bool), blocks_num, fd);
	fread(&inode_bitmap, sizeof(bool), inodes_num, fd);
	fread(&_inode_table, inode_bytes, inodes_num, fd);
	fread(&current_dir, dir_bytes, 1, fd);
	fclose(fd);

	//初始化管理员
	_user[0].model = ADMIN; 
	_user[0].uid = 341496;
	strcpy(_user[0].password, "123456");
	//创建系统用户
	_user[1].model = CASUAL;
	_user[1].uid = 740186;
	strcpy(_user[1].password, "123456");

	//初始化命令名结构组
	strcpy(_order[0].oreder_name, "format");
	strcpy(_order[1].oreder_name, "info");
	strcpy(_order[2].oreder_name, "cd");
	strcpy(_order[3].oreder_name, "dir");
	strcpy(_order[4].oreder_name, "md");
	strcpy(_order[5].oreder_name, "rd");
	strcpy(_order[6].oreder_name, "newfile");
	strcpy(_order[7].oreder_name, "cat");
	strcpy(_order[8].oreder_name, "copy");
	strcpy(_order[9].oreder_name, "del");
	strcpy(_order[10].oreder_name, "check");
	strcpy(_order[11].oreder_name, "exit");
	strcpy(_order[12].oreder_name, "help");

	//初始化路径
	strcpy(current_path, "root>");
}