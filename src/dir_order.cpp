#define _CRT_SECURE_NO_WARNINGS
#include"dir_order.h"

bool find_dir_in_dir(dir& temp, char* name, int& file_pos)
{
	int temp_size = temp.size;
	for (int i = 0; i < temp_size; i++)
	{
		if (strcmp(temp.file_entry[i].name, name) == 0 &&
			_inode_table[temp.file_entry[i].inode].i_model == _directory)
		{
			errno_t err = fopen_s(&fd, "file_control_system", "rb+");
			if (err != 0)
			{
				strcat(print_data, "file_control_system open fail!\n");
				exit(0);
			}
			fseek(fd, _inode_table[temp.file_entry[i].inode].i_addr, SEEK_SET);
			fread(&temp, sizeof(dir), 1, fd);
			file_pos = i;
			fclose(fd);
			return true;
		}
	}
	return false;
}


bool from_path_get_dir(char* path_and_dir_name, dir& dir_buf)
{
	if (strcmp(path_and_dir_name, "..") == 0)       //路径为父目录直接返回父目录的结点号
	{
		errno_t err = fopen_s(&fd, "file_control_system", "rb+");
		if (err != 0)
		{
			strcat(print_data, "file_control_system open fail!\n");
			exit(0);
		}

		fseek(fd, _inode_table[current_dir.file_entry[0].inode].i_addr, SEEK_SET);
		fread(&dir_buf, dir_bytes, 1, fd);
		fclose(fd);
		return true;
	}
	else if (strcmp(path_and_dir_name, ".") == 0)   //路径为当前目录直接返回当前的结点号
	{
		dir_buf = current_dir;
		return true;
	}
	else                                           //当路径为相对路径或绝对路径时
	{
		char* path_buf = new char[1024];
		strcpy(path_buf, path_and_dir_name);
		const char s[2] = "/";
		char* token;
		char* buf = new char[1024];
		for (int i = 0; i < 1024; i++)
			buf[i] = 0;
		int pos = 0;                               //子目录在父目录.file_entry中的位置
		/* 获取第一个文件名 */
		token = strtok_s(path_buf, s, &buf);
		if (strcmp(token, ".") == 0) {             //如果是相对路径(当前目录）
			dir_buf = current_dir;
			token = strtok_s(NULL, s, &buf);       //获取下一个文件名（因为已知要查找目录的结点号即当前目录)
			/* 继续获取其他的文件名 */

			while (token != NULL) {
				if (!find_dir_in_dir(dir_buf, token, pos))return false;
				token = strtok_s(NULL, s, &buf);
			}
			return true;
		}
		else if (strcmp(token, "..") == 0)  //如果是相对路径（父目录）
		{
			/*从当前目录找到父目录再按照路径找其它文件名*/
			dir_buf = current_dir;
			/* 继续获取其他的文件名 */
			while (token != NULL) {
				if (!find_dir_in_dir(dir_buf, token, pos))return false;  //未找到对应文件
				token = strtok_s(NULL, s, &buf);
			}
			return true;
		}
		else                              //如果是绝对路径
		{
			//提取根目录
			errno_t err = fopen_s(&fd, "file_control_system", "rb+");
			if (err != 0)
			{
				strcat(print_data, "file_control_system open fail!\n");
				exit(0);
			}
			fseek(fd, _inode_table[0].i_addr, SEEK_SET);
			fread(&dir_buf, sizeof(dir), 1, fd);
			fclose(fd);
			token = strtok_s(NULL, s, &buf);     //提取根目录的下一个目录名
			while (token != NULL) {
				if (!find_dir_in_dir(dir_buf, token, pos))return false;
				token = strtok_s(NULL, s, &buf);
			}
			return true;
		}
		delete []path_buf;
		delete[]buf;
	}
	return false;            //多此一举，纯粹为了不警告
}


void change_dir(char* path_and_dirname)
{
	dir temp;
	if (from_path_get_dir(path_and_dirname, temp))
	{
		current_dir = temp;
		get_dir_path(current_dir);
	}
	else {
		strcat(print_data, "change dir false!\n");
		return ;
	}
	return ;
}


void get_dir_path(dir temp)
{
	dir father_dir;
	if (temp.inode != 0)      //判断是否为根目录
	{
		errno_t err = fopen_s(&fd, "file_control_system", "rb+");
		if (err != 0)         
		{
			strcat(print_data, "open file_control_system fail!\n");
			exit(0);
		}
		fseek(fd, _inode_table[temp.file_entry[0].inode].i_addr, SEEK_SET);
		fread(&father_dir, dir_bytes, 1, fd);                        //提取父目录
		fclose(fd);
		get_dir_path(father_dir);           //递归调用函数
	}
	//将当前目录名加入路径中
	if (temp.inode == 0)
	{
		strcpy(current_path, temp.dir_name);  
		if(temp.inode == current_dir.inode)
			sprintf(current_path, "%s>", current_path);
	}
	else
	{
		sprintf(current_path, "%s/%s", current_path, temp.dir_name);
		if (temp.inode == current_dir.inode)
			sprintf(current_path, "%s>", current_path);
	}
}


void display(dir temp)
{
	sprintf(print_data, "%s%15s%10s%15s  %15s  %-30s\n", print_data, "物理地址", "类型", "长度(字节)", "名字", "权限");
	for (int i = 0; i < temp.size; i++)
	{
		sprintf(print_data, "%s%15ld", print_data, _inode_table[temp.file_entry[i].inode].i_addr);
		if (_inode_table[temp.file_entry[i].inode].i_model == _directory)
		{
			sprintf(print_data, "%s%10s", print_data, "<DIR>");
			sprintf(print_data, "%s%15s", print_data, "");
		}
		else
		{
			sprintf(print_data, "%s%10s", print_data, "<FILE>");
			sprintf(print_data, "%s%15u", print_data, _inode_table[temp.file_entry[i].inode].i_size);
		}
		sprintf(print_data, "%s  %15s", print_data, temp.file_entry[i].name);
		limit(_inode_table[temp.file_entry[i].inode].i_limit);
	}
}

/*
 功能：输出文件的权限
*/
void limit(int lim)
{
	if(lim == 1)
		sprintf(print_data, "%s  %-30s\n", print_data, "R");
	else if(lim == 2)
		sprintf(print_data, "%s  %-30s\n", print_data, "W");
	else
		sprintf(print_data, "%s  %-30s\n", print_data, "RW");
}

bool display_dir(char* path)
{
	dir temp;
	if (from_path_get_dir(path, temp))
	{
		sprintf(print_data, "%s%8s的目录\n\n", print_data, path);
		display(temp);
		return true;
	}
	else
		strcat(print_data, "显示目录失败！\n");
	return false;
}


void divide_PathAndName(char* source, char* path, char* file_name)
{
	char* file_name_pos;
	file_name_pos = strrchr(source, '/');  //获取文件在路径中的位置
	if (file_name_pos == NULL)          //当路径为文件名时，默认对当前路径进行操作
	{
		strcpy(path, ".");
		strcpy(file_name, source);
		return;
	}
	int path_len = file_name_pos - source;
	for (int i = 0; i < path_len; i++)          //提取路径
		path[i] = source[i];
	path[path_len] = '\0';
	int len = strlen(source);
	int j = 0;
	for (int i = path_len + 1; i < len; i++, j++)  //提取文件名
		file_name[j] = source[i];
	file_name[j] = '\0';
}


void creat_dir(char* path_and_dirname)
{
	char* dir_name = new char[max_file_name_len];
	char* path = new char[1024];
	memset(dir_name, 0, max_file_name_len);
	memset(path, 0, 1024);
	divide_PathAndName(path_and_dirname, path, dir_name);
	dir make_in_dir;
	int pos = -1;
	if (from_path_get_dir(path, make_in_dir))      //根据路径寻找创建所在目录
	{
		if (find_dir_in_dir(make_in_dir, dir_name, pos))   //查找目录中是否存在同名目录
		{
			sprintf(print_data, "%s %s 目录下已存在名为 % s 的文件，文件创建失败!\n", print_data, path, dir_name);
			return;
		}
		else            //创建目录
		{
			long addr = -1;	//分配目录在数据区的首地址
			int _inode = -1;	//分配目录的inode
			int b_index;	//分配的数据块在数据块位图的起始位置
			addr = blocks_allocation(dir_sizeof_block, b_index);   //分配数据块（函数内会修改超级快）
			if (addr < 0)
			{
				strcat(print_data, "硬盘空间不够，目录创建失败!\n");
				return;
			}
			else
			{
				_inode = inode_allocation();
				if (_inode < 0)
				{
					strcat(print_data, "结点分配失败，目录创建失败！\n");
					free_blocks(dir_sizeof_block, b_index);   //释放先前分配的数据块
					return;
				}
				//目录内容
				dir create_dir;
				strcpy(create_dir.dir_name, dir_name);
				create_dir.inode = (unsigned int)_inode;
				create_dir.size = 2;
				create_dir.file_entry[0].inode = make_in_dir.inode;
				strcpy(create_dir.file_entry[0].name, "..");
				create_dir.file_entry[1].inode = (unsigned int)_inode;
				strcpy(create_dir.file_entry[1].name, ".");
				//索引结点信息
				_inode_table[_inode].i_addr = addr;
				_inode_table[_inode].i_blocks = dir_sizeof_block;
				_inode_table[_inode].i_change = false;
				_inode_table[_inode].i_limit = rw;
				_inode_table[_inode].i_model = _directory;
				_inode_table[_inode].i_size = sizeof(dir);
				_inode_table[_inode].i_uid = uid;
				//修改父目录信息
				strcpy(make_in_dir.file_entry[make_in_dir.size].name, dir_name);
				make_in_dir.file_entry[make_in_dir.size].inode = (unsigned int)_inode;
				make_in_dir.size += 1;
				if (make_in_dir.inode == current_dir.inode)   //若是在当前目录下创建，则需要更新当前目录的信息
					current_dir = make_in_dir;
				errno_t err = fopen_s(&fd, "file_control_system", "rb+");
				if (err != 0)
				{
					strcat(print_data, "file_control_system open fail!\n");
					exit(0);
				}
				fseek(fd, 0, SEEK_SET);
				fwrite(&_super_block, superblock_bytes, 1, fd);      //写超级快
				fseek(fd, sizeof(super_block) + sizeof(bool) * b_index, SEEK_SET);
				fwrite(&block_bitmap[b_index], sizeof(bool), dir_sizeof_block, fd);  //写数据块图
				fseek(fd, sizeof(super_block) + sizeof(bool) * (blocks_num + _inode), SEEK_SET);
				fwrite(&inode_bitmap[_inode], sizeof(bool), 1, fd);            //写结点位图
				fseek(fd, sizeof(super_block) + sizeof(bool) * (blocks_num + inodes_num) + sizeof(inode) * _inode, SEEK_SET);
				fwrite(&_inode_table[_inode], inode_bytes, 1, fd);   //写索引结点
				fseek(fd, addr, SEEK_SET);
				fwrite(&create_dir, dir_bytes, 1, fd);              //将创建目录写入文件
				fseek(fd, _inode_table[make_in_dir.inode].i_addr, SEEK_SET);
				fwrite(&make_in_dir, dir_bytes, 1, fd);             //将父目录重写入文件
				fclose(fd);
				sprintf(print_data, "%s创建%s目录成功\n", print_data, dir_name);
			}
		}
	}
	delete []path;
	delete []dir_name;
}


bool if_dir_in_dir(dir destination, dir source)
{
	while (destination.inode != 0)
	{
		fseek(fd, _inode_table[destination.file_entry[0].inode].i_addr, SEEK_SET);
		fread(&destination, sizeof(dir), 1, fd);
		if (destination.inode == source.inode)return true;
	}
	return false;
}


void remove_dir(char* path_and_dirname)
{
	char* dir_name = new char[max_file_name_len];
	char* path = new char[1024];
	divide_PathAndName(path_and_dirname, path, dir_name);
	dir remove_in_dir;
	if (from_path_get_dir(path, remove_in_dir))      //根据路径寻找删除目录的父目录
	{
		dir rm_dir = remove_in_dir;
		int rm_dir_pos = -1;
		if (!find_dir_in_dir(rm_dir, dir_name, rm_dir_pos))           //查找删除目录所在目录是否存在删除目录
		{
			sprintf(print_data, "%s%s目录不存在\n", print_data, dir_name);
			fclose(fd);
			writeSession(SESSION_ADDR, 0);            //通知shell关闭会话
			return;
		}
		else
		{
			errno_t err = fopen_s(&fd, "file_control_system", "rb+");
			if (err != 0)
			{
				writeSession(SESSION_ADDR, 0);
				writeData(DATA_ADDR, print_data);
				writeFlag(FLAG_ADDR, 1);
				strcat(print_data, "file_control_system open fail!\n");
				exit(0);
			}
			if (if_dir_in_dir(current_dir, rm_dir))       //删除目录是否包含当前目录
			{
				strcat(print_data, "删除失败，有进程正在使用删除目录中的文件！\n\0");
				fclose(fd);
				writeSession(SESSION_ADDR, 0);
				return;
			}
			if (rm_dir.size > 2)            //目录不为空
			{
				sprintf(print_data, "%s%s 该目录下有文件，是否还要删除(y/n)\n\0", print_data, dir_name);
				flushData(DATA_ADDR);
				writeData(DATA_ADDR, print_data);
				writeFlag(FLAG_ADDR, 1);
				char flag;
				while (true)               //等待shell写进文件内容
				{
					readFlag(FLAG_ADDR, flag);
					if (flag == 0)break;
				}
				char* buffer1 = new char[100];
				memset(buffer1, 0, 100);
				readData(DATA_ADDR, buffer1);   //从共享内存中读取选择
				writeSession(SESSION_ADDR, 0);    //会话结束
				if (strcmp(buffer1, "y") == 0)
				{
					remove_dir(remove_in_dir, rm_dir);	//递归删除子文件(文件和目录)
					for (int i = rm_dir_pos; i < remove_in_dir.size - 1; i++)   //修改父目录
					{
						remove_in_dir.file_entry[i].inode = remove_in_dir.file_entry[i + 1].inode;
						strcpy(remove_in_dir.file_entry[i].name, remove_in_dir.file_entry[i + 1].name);
					}
					remove_in_dir.size--;
					if (remove_in_dir.inode == current_dir.inode)
						current_dir = remove_in_dir;
					fseek(fd, _inode_table[remove_in_dir.inode].i_addr, SEEK_SET);   //将父目录写回系统文件
					fwrite(&remove_in_dir, dir_bytes, 1, fd);
					fclose(fd);
					sprintf(print_data, "%s目录删除成功\n", dir_name);        //覆盖前面的写进共享空间的内容
					return;
				}
				else
				{
					strcpy(print_data, "目录删除终止!\n");     //覆盖前面的写进共享空间的内容
					fclose(fd);
					return;
				}
			}
			else            //目录为空
			{
				free_blocks(dir_sizeof_block, (_inode_table[rm_dir.inode].i_addr - first_data_addr) / block_size);
				free_inode(rm_dir.inode);      //释放该目录数据块和位图成功
				for (int i = rm_dir_pos; i < remove_in_dir.size - 1; i++)   //修改父目录
				{
					remove_in_dir.file_entry[i].inode = remove_in_dir.file_entry[i + 1].inode;
					strcpy(remove_in_dir.file_entry[i].name, remove_in_dir.file_entry[i + 1].name);
				}
				remove_in_dir.size--;
				if (remove_in_dir.inode == current_dir.inode)
					current_dir = remove_in_dir;
				fseek(fd, _inode_table[remove_in_dir.inode].i_addr, SEEK_SET);   //将父目录写回系统文件
				fwrite(&remove_in_dir, dir_bytes, 1, fd);
				fclose(fd);
				writeSession(SESSION_ADDR, 0);    //会话结束
				sprintf(print_data, "%s%s目录删除成功\n", print_data, dir_name);
				return;
			}

		}
	}
	delete []path;
	delete []dir_name;
}


void remove_dir(dir father_dir, dir rm_dir)
{
	int i;
	for (i = 2; i < rm_dir.size; i++)          //循环删除子文件
	{
		if (_inode_table[rm_dir.file_entry[i].inode].i_model == _directory)    //目录
		{
			dir now_dir;
			fseek(fd, _inode_table[rm_dir.file_entry[i].inode].i_addr, SEEK_SET);
			fread(&now_dir, dir_bytes, 1, fd);
			remove_dir(rm_dir, now_dir);
		}
		else
			delete_file(rm_dir, i);             //文件
	}
	free_blocks(dir_sizeof_block, (_inode_table[rm_dir.inode].i_addr - first_data_addr) / block_size);
	free_inode(rm_dir.inode);      //释放数据块和位图成功
}