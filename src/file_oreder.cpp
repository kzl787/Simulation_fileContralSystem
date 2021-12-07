#define _CRT_SECURE_NO_WARNINGS
#include"file_oreder.h"

bool find_file_in_dir(dir temp, char* name, int &pos)
{
	int temp_size = temp.size;
	for (int i = 0; i < temp_size; i++)       //��������
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
	if (from_path_get_dir(path, del_in_dir))      //����·��Ѱ���ļ�����Ŀ¼
	{
		if (!find_file_in_dir(del_in_dir, file_name, file_pos))  //��ѯ�����ļ��Ƿ����
		{
			sprintf(print_data, "%s%s Ŀ¼�²�������Ϊ %s ���ļ�!\n", print_data, del_in_dir.dir_name, file_name);
			return;
		}
		errno_t err = fopen_s(&fd, "file_control_system", "wb+");
		if (err != 0)
		{
			strcat(print_data, "file_control_system open fail!\n");
			exit(0);
		}
		delete_file(del_in_dir, file_pos);   //ɾ���ļ�
		//�޸ĸ�Ŀ¼
		for (int i = file_pos; i < del_in_dir.size - 1; i++)       //�޸ĸ�Ŀ¼�����ļ�����
		{
			del_in_dir.file_entry[i].inode = del_in_dir.file_entry[i + 1].inode;
			strcpy(del_in_dir.file_entry[i].name, del_in_dir.file_entry[i + 1].name);
		}
		del_in_dir.size--;
		if (del_in_dir.inode == current_dir.inode)
			current_dir = del_in_dir;
		fseek(fd, _inode_table[del_in_dir.file_entry[1].inode].i_addr, SEEK_SET);  //��Ŀ¼д��ϵͳ
		fwrite(&del_in_dir, dir_bytes, 1, fd);
		fclose(fd);
		delete []path;
		delete []file_name;
		return;
	}
	else
	{
		strcat(print_data, "·������\n");
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
	if (from_path_get_dir(path, create_in_dir))      //����·��Ѱ���ļ�����Ŀ¼
	{
		if (find_file_in_dir(create_in_dir, file_name, file_pos))  //��ѯ�����ļ����Ƿ��������
		{
			sprintf(print_data, "%s%s Ŀ¼���Ѵ�����Ϊ %s ���ļ����ļ�����ʧ��!\n", print_data, create_in_dir.dir_name, file_name);
			writeSession(SESSION_ADDR, 0);
			return;
		}
		char ch;                          //��ȡ�ַ�
		int size = 1024*1024*10, count = 0;         //��������С���ļ���С
		char* buffer = new char[size];       //������
		for (int i = 0; i < size; i++)
			buffer[i] = '0';
		strcpy(print_data,"�������ļ����ݣ�����Enter + #���������룺\n\n");
		flushData(DATA_ADDR);
		writeData(DATA_ADDR, print_data);
		writeFlag(FLAG_ADDR, 1);
		char flag;
		while (true)               //�ȴ�shellд���ļ�����
		{
			readFlag(FLAG_ADDR, flag);
			if (flag == 0)break;
		}
		readData(DATA_ADDR, buffer);              //��ȡ�ļ�����
		count = strlen(buffer);
		buffer[count] = '\0';                              //���ļ�������
		int b_num = (count + 1) / 1024;
		if (((count + 1) % 1024))b_num++;          //�ļ���ռ���ݿ���
		int b_index = -1;         //�ļ���ռ���ݿ�ĵ�һ��λ��
		long addr = -1;           //�ļ����׵�ַ
		int inode_index = -1;         //�ļ�����������
		addr = blocks_allocation(b_num, b_index);
		if (addr < 0)          //����ռ�ʧ��
		{
			strcpy(print_data, "�ļ�̫��Ӳ�̿ռ䲻�����ļ�����ʧ��!\n");
			writeSession(SESSION_ADDR, 0);
			return;
		}
		inode_index = inode_allocation();
		if (inode_index < 0)
		{
			strcpy(print_data, "������ʧ��!\n");
			writeSession(SESSION_ADDR, 0);
			return;
		}
		//д���ļ����������Ϣ
		_inode_table[inode_index].i_addr = addr;
		_inode_table[inode_index].i_blocks = (unsigned int)b_num;
		_inode_table[inode_index].i_change = false;
		_inode_table[inode_index].i_limit = rw;
		_inode_table[inode_index].i_model = _file;
		_inode_table[inode_index].i_size = (unsigned int)count;
		//�����ļ�����Ŀ¼��Ϣ
		create_in_dir.file_entry[create_in_dir.size].inode = (unsigned int)inode_index;
		strcpy(create_in_dir.file_entry[create_in_dir.size].name, file_name);
		create_in_dir.size++;
		//��ϵͳ�ļ�
		errno_t err = fopen_s(&fd, "file_control_system", "rb+");
		if (err != 0)
		{
			strcpy(print_data, "file_control_system open fail!\n");
			writeSession(SESSION_ADDR, 0);
			writeFlag(FLAG_ADDR, 1);
			exit(0);
		}
		//д���ļ�����Ϣ
		fseek(fd, 0, SEEK_SET);
		fwrite(&_super_block, superblock_bytes, 1, fd);   //������
		fseek(fd, superblock_bytes + sizeof(bool) * b_index, SEEK_SET);
		fwrite(&block_bitmap[b_index], sizeof(bool), b_num, fd);         //���ݿ�ͼ
		fseek(fd, superblock_bytes + sizeof(bool) * (blocks_num + inode_index), SEEK_SET);
		fwrite(&inode_bitmap[inode_index], sizeof(bool), 1, fd);          //���λͼ
		fseek(fd, superblock_bytes + sizeof(bool) * (blocks_num + inodes_num) + inode_bytes * inode_index, SEEK_SET);
		fwrite(&_inode_table[inode_index], inode_bytes, 1, fd);         //�������
		fseek(fd, addr, SEEK_SET);
		fwrite(buffer, count, 1, fd);                     //�ļ�����
		fseek(fd, _inode_table[create_in_dir.inode].i_addr, SEEK_SET);
		fwrite(&create_in_dir, dir_bytes, 1, fd);               //��Ŀ¼
		fclose(fd);
		if (current_dir.inode == create_in_dir.inode)
			current_dir = create_in_dir;
		writeSession(SESSION_ADDR, 0);             //�رջỰ
		strcpy(print_data, "�ļ������ɹ���\n");
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
	if (from_path_get_dir(path, open_in_dir))      //����·��Ѱ���ļ�����Ŀ¼
	{
		if (!find_file_in_dir(open_in_dir, file_name, file_pos))  //��ѯ�����ļ��Ƿ����
		{
			sprintf(print_data, "%s%s Ŀ¼�²�������Ϊ %s ���ļ�!\n", print_data, open_in_dir.dir_name, file_name);
			return;
		}
		unsigned int inode_index = open_in_dir.file_entry[file_pos].inode;
		char* buf = new char[_inode_table[inode_index].i_size + 1];       //������
		memset(buf, 0, _inode_table[inode_index].i_size+1);
		errno_t err = fopen_s(&fd, "file_control_system", "rb");
		if (err != 0)
		{
			printf("file_control_system open fail!\n");
			exit(0);
		}
		fseek(fd, _inode_table[inode_index].i_addr, SEEK_SET);
		fread(buf, sizeof(char), _inode_table[inode_index].i_size, fd);      //��ȡ�ļ�  ��������������
		fclose(fd);
		//�洢�ļ�ʱ�����ļ���β��'\0'               buffer[_inode_table[inode_index].i_size] = '\0';
		sprintf(print_data, "%s�ļ� %s ����������:\n\n", print_data, file_name);
		strcat(print_data, buf);
		delete []buf;
		delete []file_name;
		delete []path;
	}
}


void copy_file(char* source_path, char* des_path)
{
	char* buf;                     //������
	long flen = 0;                  //�ļ�����
	char file_name[max_file_name_len];    //�ļ���
	if (enviroment(source_path))                 //��Windows������linux
	{
		dir copy_to_dir;  //�������ļ�����Ŀ¼
		errno_t err = fopen_s(&fd, source_path, "rb+");
		if (err != 0)
		{
			strcat(print_data, "Windows�в������ļ�������ʧ�ܣ�\n");
			return;
		}
		fseek(fd, 0, SEEK_END);          //���嵽�ļ���β
		flen = ftell(fd);              //��ȡ�ļ�����
		buf = new char[flen + 1];
		fseek(fd, 0, SEEK_SET);
		fread(buf, sizeof(char), flen, fd);             //��ȡ�ļ�
		buf[flen] = '\0';
		fclose(fd);
		char div = '\\';
		strcpy(file_name, strrchr(source_path, div) + 1);
		if (from_path_get_dir(des_path, copy_to_dir))
		{
			write_file(copy_to_dir, file_name, buf, flen);
			delete[] buf;
			sprintf(print_data, "%s%s�ļ������ɹ���\n", print_data, file_name);
			if (copy_to_dir.inode == current_dir.inode)
				current_dir = copy_to_dir;
			return;
		}
		else
			strcat(print_data, "�ļ�����ʧ�ܣ�\n");
		delete[] buf;
	}
	else if (enviroment(des_path))      //��Linux������windows
	{
		dir copy_from_dir;                 //�������ļ�����Ŀ¼
		char file_name[max_file_name_len];    //�ļ���
		char path[1024];             //�����ļ�����Ŀ¼
		divide_PathAndName(source_path, path, file_name);
		if (from_path_get_dir(path, copy_from_dir))      //����·��Ѱ���ļ�����Ŀ¼
		{
			int file_pos = -1;
			if (find_file_in_dir(copy_from_dir, file_name, file_pos))  //��ѯ�ļ��Ƿ����
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
				fread(buf, sizeof(char), flen, fd);               //��Linux��ȡ�ļ�
				fclose(fd);
				int a = strlen(des_path), b = strlen(file_name);
				char* des_PathAndName = new char[a + b + 1];
				memset(des_PathAndName, 0, a + b + 1);
				sprintf(des_PathAndName, "%s\\%s", des_path, file_name);
				err = fopen_s(&fd, des_PathAndName, "wb");
				if (err != 0)
				{
					strcat(print_data, "��Window�����ļ�ʧ�ܣ�\n");
					fclose(fd);
					return;
				}
				fwrite(buf, sizeof(char), flen, fd);
				fclose(fd);
				sprintf(print_data, "%s%s�ļ������ɹ���\n", print_data, file_name);
				return;
			}
			else
			{
				sprintf(print_data, "%s%sĿ¼�в�����%s�ļ�", print_data, copy_from_dir.dir_name, file_name);
				return;
			}
		}
	}
	else             //�ڲ��ļ�֮��Ŀ���
	{
		dir copy_from_dir, copy_to_dir;
		char file_name[max_file_name_len];    //�ļ���
		char path[1024];             //�����ļ�����Ŀ¼
		divide_PathAndName(source_path, path, file_name);
		if (from_path_get_dir(path, copy_from_dir))      //����·��Ѱ���ļ�����Ŀ¼
		{
			int file_pos = -1;
			if (find_file_in_dir(copy_from_dir, file_name, file_pos))  //��ѯ�ļ��Ƿ����
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
				from_path_get_dir(des_path, copy_to_dir);          //����Ŀ��Ŀ¼
				if (!find_file_in_dir(copy_to_dir, file_name, file_pos))   //����Ŀ¼�в���ͬ���ļ�
				{
					write_file(copy_to_dir, file_name, buf, size);
					sprintf(print_data,"%s%s�ļ������ɹ���\n", print_data, file_name);
					if (copy_to_dir.inode == current_dir.inode)
						current_dir = copy_to_dir;
				}
				else
				{
					sprintf(print_data, "%s%sĿ¼�д���ͬ��%s�ļ���\n", print_data, copy_to_dir.dir_name, file_name);
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
	char* div = strstr(path, host);     //��·������<host>�ַ��������ַ�ָ��ָ���
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
	int b_index = -1;         //�ļ���ռ���ݿ�ĵ�һ��λ��
	long addr = -1;           //�ļ����׵�ַ
	int inode_index = -1;         //�ļ�����������
	addr = blocks_allocation(b_num, b_index);
	if (addr < 0)          //����ռ�ʧ��
	{
		strcat(print_data, "�ļ�̫��Ӳ�̿ռ䲻�����ļ�����ʧ��!\n");
		return;
	}
	inode_index = inode_allocation();
	if (inode_index < 0)
	{
		strcat(print_data, "������ʧ��!\n");
		return;
	}
	//д���ļ����������Ϣ
	_inode_table[inode_index].i_addr = addr;
	_inode_table[inode_index].i_blocks = (unsigned int)b_num;
	_inode_table[inode_index].i_change = false;
	_inode_table[inode_index].i_limit = rw;
	_inode_table[inode_index].i_model = _file;
	_inode_table[inode_index].i_size = (unsigned int)flen;
	//�����ļ�����Ŀ¼��Ϣ
	temp.file_entry[temp.size].inode = (unsigned int)inode_index;
	strcpy(temp.file_entry[temp.size].name, name);
	temp.size++;
	//д��Ӳ��
	errno_t err = fopen_s(&fd, "file_control_system", "rb+");
	if (err != 0)
	{
		strcat(print_data, "file_control_system open fail!\n");
		exit(0);
	}
	fseek(fd, 0, SEEK_SET);
	fwrite(&_super_block, superblock_bytes, 1, fd);   //������
	fseek(fd, superblock_bytes + sizeof(bool) * b_index, SEEK_SET);
	fwrite(&block_bitmap[b_index], sizeof(bool), b_num, fd);         //���ݿ�ͼ
	fseek(fd, superblock_bytes + sizeof(bool) * (blocks_num + inode_index), SEEK_SET);
	fwrite(&inode_bitmap[inode_index], sizeof(bool), 1, fd);          //���λͼ
	fseek(fd, superblock_bytes + sizeof(bool) * (blocks_num + inodes_num) + inode_bytes * inode_index, SEEK_SET);
	fwrite(&_inode_table[inode_index], inode_bytes, 1, fd);         //�������
	fseek(fd, addr, SEEK_SET);
	fwrite(buf, sizeof(char), flen, fd);                     //�ļ�����
	fseek(fd, _inode_table[temp.inode].i_addr, SEEK_SET);
	fwrite(&temp, dir_bytes, 1, fd);               //��Ŀ¼
	fclose(fd);
}