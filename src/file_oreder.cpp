#define _CRT_SECURE_NO_WARNINGS
#include"file_oreder.h"

bool find_file_in_dir(dir temp, char* name, int &pos)
{
	int temp_size = temp.size;
	for (int i = 0; i < temp_size; i++)       //迭代查找
	{
		if (strcmp(temp.file_entry[i].name, name) == 0 &&
			_inode_table[temp.file_entry[i].inode].i_model == _file)
		{
			pos = i;
			return true;
		}
	}
	pos = -1;
	return false;
}


void del_file(char* path_and_filename)
{
	char* file_name = new char[max_file_name_len];
	char* path = new char[1024];
	divide_PathAndName(path_and_filename, path, file_name);
	dir del_in_dir;
	int file_pos = -1;
	if (from_path_get_dir(path, del_in_dir))      //根据路径寻找文件所在目录
	{
		if (!find_file_in_dir(del_in_dir, file_name, file_pos))  //查询创建文件是否存在
		{
			sprintf(print_data, "%s%s 目录下不存在名为 %s 的文件!\n", print_data, del_in_dir.dir_name, file_name);
			return;
		}
		errno_t err = fopen_s(&fd, "file_control_system", "wb+");
		if (err != 0)
		{
			strcat(print_data, "file_control_system open fail!\n");
			exit(0);
		}
		delete_file(del_in_dir, file_pos);   //删除文件
		//修改父目录
		for (int i = file_pos; i < del_in_dir.size - 1; i++)       //修改父目录的子文件数组
		{
			del_in_dir.file_entry[i].inode = del_in_dir.file_entry[i + 1].inode;
			strcpy(del_in_dir.file_entry[i].name, del_in_dir.file_entry[i + 1].name);
		}
		del_in_dir.size--;
		if (del_in_dir.inode == current_dir.inode)
			current_dir = del_in_dir;
		fseek(fd, _inode_table[del_in_dir.file_entry[1].inode].i_addr, SEEK_SET);  //父目录写回系统
		fwrite(&del_in_dir, dir_bytes, 1, fd);
		fclose(fd);
		delete []path;
		delete []file_name;
		return;
	}
	else
	{
		strcat(print_data, "路径错误！\n");
		return;
	}
}


void delete_file(dir remove_in_dir, int file_pos)
{
	int f_inode = remove_in_dir.file_entry[file_pos].inode;
	free_blocks(_inode_table[f_inode].i_blocks,
		(_inode_table[f_inode].i_addr - first_data_addr) / block_size);
	free_inode(f_inode);
}


void create_file(char* path_and_filename)
{
	char* file_name = new char[max_file_name_len];
	char* path = new char[1024];
	divide_PathAndName(path_and_filename, path, file_name);
	dir create_in_dir;
	int file_pos = -1;
	if (from_path_get_dir(path, create_in_dir))      //根据路径寻找文件所在目录
	{
		if (find_file_in_dir(create_in_dir, file_name, file_pos))  //查询创建文件名是否存在重名
		{
			sprintf(print_data, "%s%s 目录下已存在名为 %s 的文件，文件创建失败!\n", print_data, create_in_dir.dir_name, file_name);
			writeSession(SESSION_ADDR, 0);
			return;
		}
		char ch;                          //读取字符
		int size = 1024*1024*10, count = 0;         //缓冲区大小，文件大小
		char* buffer = new char[size];       //缓冲区
		for (int i = 0; i < size; i++)
			buffer[i] = '0';
		strcpy(print_data,"请输入文件内容，按（Enter + #）结束输入：\n\n");
		flushData(DATA_ADDR);
		writeData(DATA_ADDR, print_data);
		writeFlag(FLAG_ADDR, 1);
		char flag;
		while (true)               //等待shell写进文件内容
		{
			readFlag(FLAG_ADDR, flag);
			if (flag == 0)break;
		}
		readData(DATA_ADDR, buffer);              //读取文件内容
		count = strlen(buffer);
		buffer[count] = '\0';                              //加文件结束符
		int b_num = (count + 1) / 1024;
		if (((count + 1) % 1024))b_num++;          //文件所占数据块数
		int b_index = -1;         //文件所占数据块的第一个位置
		long addr = -1;           //文件的首地址
		int inode_index = -1;         //文件的索引结点号
		addr = blocks_allocation(b_num, b_index);
		if (addr < 0)          //分配空间失败
		{
			strcpy(print_data, "文件太大，硬盘空间不够，文件创建失败!\n");
			writeSession(SESSION_ADDR, 0);
			return;
		}
		inode_index = inode_allocation();
		if (inode_index < 0)
		{
			strcpy(print_data, "结点分配失败!\n");
			writeSession(SESSION_ADDR, 0);
			return;
		}
		//写入文件索引结点信息
		_inode_table[inode_index].i_addr = addr;
		_inode_table[inode_index].i_blocks = (unsigned int)b_num;
		_inode_table[inode_index].i_change = false;
		_inode_table[inode_index].i_limit = rw;
		_inode_table[inode_index].i_model = _file;
		_inode_table[inode_index].i_size = (unsigned int)count;
		//更新文件所在目录信息
		create_in_dir.file_entry[create_in_dir.size].inode = (unsigned int)inode_index;
		strcpy(create_in_dir.file_entry[create_in_dir.size].name, file_name);
		create_in_dir.size++;
		//打开系统文件
		errno_t err = fopen_s(&fd, "file_control_system", "rb+");
		if (err != 0)
		{
			strcpy(print_data, "file_control_system open fail!\n");
			writeSession(SESSION_ADDR, 0);
			writeFlag(FLAG_ADDR, 1);
			exit(0);
		}
		//写回文件的信息
		fseek(fd, 0, SEEK_SET);
		fwrite(&_super_block, superblock_bytes, 1, fd);   //超级快
		fseek(fd, superblock_bytes + sizeof(bool) * b_index, SEEK_SET);
		fwrite(&block_bitmap[b_index], sizeof(bool), b_num, fd);         //数据块图
		fseek(fd, superblock_bytes + sizeof(bool) * (blocks_num + inode_index), SEEK_SET);
		fwrite(&inode_bitmap[inode_index], sizeof(bool), 1, fd);          //结点位图
		fseek(fd, superblock_bytes + sizeof(bool) * (blocks_num + inodes_num) + inode_bytes * inode_index, SEEK_SET);
		fwrite(&_inode_table[inode_index], inode_bytes, 1, fd);         //索引结点
		fseek(fd, addr, SEEK_SET);
		fwrite(buffer, count, 1, fd);                     //文件内容
		fseek(fd, _inode_table[create_in_dir.inode].i_addr, SEEK_SET);
		fwrite(&create_in_dir, dir_bytes, 1, fd);               //父目录
		fclose(fd);
		if (current_dir.inode == create_in_dir.inode)
			current_dir = create_in_dir;
		writeSession(SESSION_ADDR, 0);             //关闭会话
		strcpy(print_data, "文件创建成功！\n");
		delete []path;
		delete []file_name;
		delete []buffer;
	}
}


void open_file(char* path_and_filename)
{
	char* file_name = new char[max_file_name_len];
	char* path = new char[1024];
	divide_PathAndName(path_and_filename, path, file_name);
	dir open_in_dir;
	int file_pos = -1;
	if (from_path_get_dir(path, open_in_dir))      //根据路径寻找文件所在目录
	{
		if (!find_file_in_dir(open_in_dir, file_name, file_pos))  //查询创建文件是否存在
		{
			sprintf(print_data, "%s%s 目录下不存在名为 %s 的文件!\n", print_data, open_in_dir.dir_name, file_name);
			return;
		}
		unsigned int inode_index = open_in_dir.file_entry[file_pos].inode;
		char* buf = new char[_inode_table[inode_index].i_size + 1];       //缓冲区
		memset(buf, 0, _inode_table[inode_index].i_size+1);
		errno_t err = fopen_s(&fd, "file_control_system", "rb");
		if (err != 0)
		{
			printf("file_control_system open fail!\n");
			exit(0);
		}
		fseek(fd, _inode_table[inode_index].i_addr, SEEK_SET);
		fread(buf, sizeof(char), _inode_table[inode_index].i_size, fd);      //读取文件  ？？？？？？？
		fclose(fd);
		//存储文件时已在文件结尾加'\0'               buffer[_inode_table[inode_index].i_size] = '\0';
		sprintf(print_data, "%s文件 %s 的内容如下:\n\n", print_data, file_name);
		strcat(print_data, buf);
		delete []buf;
		delete []file_name;
		delete []path;
	}
}


void copy_file(char* source_path, char* des_path)
{
	char* buf;                     //缓冲区
	long flen = 0;                  //文件长度
	char file_name[max_file_name_len];    //文件名
	if (enviroment(source_path))                 //从Windows拷贝到linux
	{
		dir copy_to_dir;  //拷贝后文件所在目录
		errno_t err = fopen_s(&fd, source_path, "rb+");
		if (err != 0)
		{
			strcat(print_data, "Windows中不存在文件，拷贝失败！\n");
			return;
		}
		fseek(fd, 0, SEEK_END);          //定义到文件结尾
		flen = ftell(fd);              //获取文件长度
		buf = new char[flen + 1];
		fseek(fd, 0, SEEK_SET);
		fread(buf, sizeof(char), flen, fd);             //读取文件
		buf[flen] = '\0';
		fclose(fd);
		char div = '\\';
		strcpy(file_name, strrchr(source_path, div) + 1);
		if (from_path_get_dir(des_path, copy_to_dir))
		{
			write_file(copy_to_dir, file_name, buf, flen);
			delete[] buf;
			sprintf(print_data, "%s%s文件拷贝成功！\n", print_data, file_name);
			if (copy_to_dir.inode == current_dir.inode)
				current_dir = copy_to_dir;
			return;
		}
		else
			strcat(print_data, "文件拷贝失败！\n");
		delete[] buf;
	}
	else if (enviroment(des_path))      //从Linux拷贝到windows
	{
		dir copy_from_dir;                 //拷贝的文件所在目录
		char file_name[max_file_name_len];    //文件名
		char path[1024];             //拷贝文件所在目录
		divide_PathAndName(source_path, path, file_name);
		if (from_path_get_dir(path, copy_from_dir))      //根据路径寻找文件所在目录
		{
			int file_pos = -1;
			if (find_file_in_dir(copy_from_dir, file_name, file_pos))  //查询文件是否存在
			{
				int inode = copy_from_dir.file_entry[file_pos].inode;
				flen = _inode_table[inode].i_size;
				buf = new char[flen];
				memset(buf, 0, flen);
				errno_t err = fopen_s(&fd, "file_control_system", "rb");
				if (err != 0)
				{
					strcat(print_data, "file_control_system open fail!\n");
					exit(0);
				}
				fseek(fd, _inode_table[inode].i_addr, SEEK_SET);
				fread(buf, sizeof(char), flen, fd);               //从Linux读取文件
				fclose(fd);
				int a = strlen(des_path), b = strlen(file_name);
				char* des_PathAndName = new char[a + b + 1];
				memset(des_PathAndName, 0, a + b + 1);
				sprintf(des_PathAndName, "%s\\%s", des_path, file_name);
				err = fopen_s(&fd, des_PathAndName, "wb");
				if (err != 0)
				{
					strcat(print_data, "向Window拷贝文件失败！\n");
					fclose(fd);
					return;
				}
				fwrite(buf, sizeof(char), flen, fd);
				fclose(fd);
				sprintf(print_data, "%s%s文件拷贝成功！\n", print_data, file_name);
				return;
			}
			else
			{
				sprintf(print_data, "%s%s目录中不存在%s文件", print_data, copy_from_dir.dir_name, file_name);
				return;
			}
		}
	}
	else             //内部文件之间的拷贝
	{
		dir copy_from_dir, copy_to_dir;
		char file_name[max_file_name_len];    //文件名
		char path[1024];             //拷贝文件所在目录
		divide_PathAndName(source_path, path, file_name);
		if (from_path_get_dir(path, copy_from_dir))      //根据路径寻找文件所在目录
		{
			int file_pos = -1;
			if (find_file_in_dir(copy_from_dir, file_name, file_pos))  //查询文件是否存在
			{
				int inode = copy_from_dir.file_entry[file_pos].inode;
				long size = _inode_table[inode].i_size;
				buf = new char[size];
				memset(buf, 0, size);
				errno_t err = fopen_s(&fd, "file_control_system", "rb");
				if (err != 0)
				{
					strcat(print_data,"file_control_system open fail!\n");
					exit(0);
				}
				fseek(fd, _inode_table[inode].i_addr, SEEK_SET);
				fread(buf, sizeof(char), size, fd);
				fclose(fd);
				from_path_get_dir(des_path, copy_to_dir);          //查找目的目录
				if (!find_file_in_dir(copy_to_dir, file_name, file_pos))   //复制目录中不存同名文件
				{
					write_file(copy_to_dir, file_name, buf, size);
					sprintf(print_data,"%s%s文件拷贝成功！\n", print_data, file_name);
					if (copy_to_dir.inode == current_dir.inode)
						current_dir = copy_to_dir;
				}
				else
				{
					sprintf(print_data, "%s%s目录中存在同名%s文件！\n", print_data, copy_to_dir.dir_name, file_name);
					return;
				}
				delete []buf;
			}
		}
	}
}


bool enviroment(char* path)
{
	char host[] = "<host>";
	char* div = strstr(path, host);     //若路径中无<host>字符串，则字符指针指向空
	if (div == path)
	{
		strcpy(path, div + sizeof(host) - 1);
		return true;
	}
	return false;
}


void write_file(dir& temp, char* name, char* buf, long flen)
{
	int b_num = flen / 1024;
	if (flen % 1024)b_num++;
	int b_index = -1;         //文件所占数据块的第一个位置
	long addr = -1;           //文件的首地址
	int inode_index = -1;         //文件的索引结点号
	addr = blocks_allocation(b_num, b_index);
	if (addr < 0)          //分配空间失败
	{
		strcat(print_data, "文件太大，硬盘空间不够，文件创建失败!\n");
		return;
	}
	inode_index = inode_allocation();
	if (inode_index < 0)
	{
		strcat(print_data, "结点分配失败!\n");
		return;
	}
	//写入文件索引结点信息
	_inode_table[inode_index].i_addr = addr;
	_inode_table[inode_index].i_blocks = (unsigned int)b_num;
	_inode_table[inode_index].i_change = false;
	_inode_table[inode_index].i_limit = rw;
	_inode_table[inode_index].i_model = _file;
	_inode_table[inode_index].i_size = (unsigned int)flen;
	//更新文件所在目录信息
	temp.file_entry[temp.size].inode = (unsigned int)inode_index;
	strcpy(temp.file_entry[temp.size].name, name);
	temp.size++;
	//写回硬盘
	errno_t err = fopen_s(&fd, "file_control_system", "rb+");
	if (err != 0)
	{
		strcat(print_data, "file_control_system open fail!\n");
		exit(0);
	}
	fseek(fd, 0, SEEK_SET);
	fwrite(&_super_block, superblock_bytes, 1, fd);   //超级快
	fseek(fd, superblock_bytes + sizeof(bool) * b_index, SEEK_SET);
	fwrite(&block_bitmap[b_index], sizeof(bool), b_num, fd);         //数据块图
	fseek(fd, superblock_bytes + sizeof(bool) * (blocks_num + inode_index), SEEK_SET);
	fwrite(&inode_bitmap[inode_index], sizeof(bool), 1, fd);          //结点位图
	fseek(fd, superblock_bytes + sizeof(bool) * (blocks_num + inodes_num) + inode_bytes * inode_index, SEEK_SET);
	fwrite(&_inode_table[inode_index], inode_bytes, 1, fd);         //索引结点
	fseek(fd, addr, SEEK_SET);
	fwrite(buf, sizeof(char), flen, fd);                     //文件内容
	fseek(fd, _inode_table[temp.inode].i_addr, SEEK_SET);
	fwrite(&temp, dir_bytes, 1, fd);               //父目录
	fclose(fd);
}