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
	if (strcmp(path_and_dir_name, "..") == 0)       //·��Ϊ��Ŀ¼ֱ�ӷ��ظ�Ŀ¼�Ľ���
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
	else if (strcmp(path_and_dir_name, ".") == 0)   //·��Ϊ��ǰĿ¼ֱ�ӷ��ص�ǰ�Ľ���
	{
		dir_buf = current_dir;
		return true;
	}
	else                                           //��·��Ϊ���·�������·��ʱ
	{
		char* path_buf = new char[1024];
		strcpy(path_buf, path_and_dir_name);
		const char s[2] = "/";
		char* token;
		char* buf = new char[1024];
		for (int i = 0; i < 1024; i++)
			buf[i] = 0;
		int pos = 0;                               //��Ŀ¼�ڸ�Ŀ¼.file_entry�е�λ��
		/* ��ȡ��һ���ļ��� */
		token = strtok_s(path_buf, s, &buf);
		if (strcmp(token, ".") == 0) {             //��������·��(��ǰĿ¼��
			dir_buf = current_dir;
			token = strtok_s(NULL, s, &buf);       //��ȡ��һ���ļ�������Ϊ��֪Ҫ����Ŀ¼�Ľ��ż���ǰĿ¼)
			/* ������ȡ�������ļ��� */

			while (token != NULL) {
				if (!find_dir_in_dir(dir_buf, token, pos))return false;
				token = strtok_s(NULL, s, &buf);
			}
			return true;
		}
		else if (strcmp(token, "..") == 0)  //��������·������Ŀ¼��
		{
			/*�ӵ�ǰĿ¼�ҵ���Ŀ¼�ٰ���·���������ļ���*/
			dir_buf = current_dir;
			/* ������ȡ�������ļ��� */
			while (token != NULL) {
				if (!find_dir_in_dir(dir_buf, token, pos))return false;  //δ�ҵ���Ӧ�ļ�
				token = strtok_s(NULL, s, &buf);
			}
			return true;
		}
		else                              //����Ǿ���·��
		{
			//��ȡ��Ŀ¼
			errno_t err = fopen_s(&fd, "file_control_system", "rb+");
			if (err != 0)
			{
				strcat(print_data, "file_control_system open fail!\n");
				exit(0);
			}
			fseek(fd, _inode_table[0].i_addr, SEEK_SET);
			fread(&dir_buf, sizeof(dir), 1, fd);
			fclose(fd);
			token = strtok_s(NULL, s, &buf);     //��ȡ��Ŀ¼����һ��Ŀ¼��
			while (token != NULL) {
				if (!find_dir_in_dir(dir_buf, token, pos))return false;
				token = strtok_s(NULL, s, &buf);
			}
			return true;
		}
		delete []path_buf;
		delete[]buf;
	}
	return false;            //���һ�٣�����Ϊ�˲�����
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
	if (temp.inode != 0)      //�ж��Ƿ�Ϊ��Ŀ¼
	{
		errno_t err = fopen_s(&fd, "file_control_system", "rb+");
		if (err != 0)         
		{
			strcat(print_data, "open file_control_system fail!\n");
			exit(0);
		}
		fseek(fd, _inode_table[temp.file_entry[0].inode].i_addr, SEEK_SET);
		fread(&father_dir, dir_bytes, 1, fd);                        //��ȡ��Ŀ¼
		fclose(fd);
		get_dir_path(father_dir);           //�ݹ���ú���
	}
	//����ǰĿ¼������·����
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
	sprintf(print_data, "%s%15s%10s%15s  %15s  %-30s\n", print_data, "�����ַ", "����", "����(�ֽ�)", "����", "Ȩ��");
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
 ���ܣ�����ļ���Ȩ��
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
		sprintf(print_data, "%s%8s��Ŀ¼\n\n", print_data, path);
		display(temp);
		return true;
	}
	else
		strcat(print_data, "��ʾĿ¼ʧ�ܣ�\n");
	return false;
}


void divide_PathAndName(char* source, char* path, char* file_name)
{
	char* file_name_pos;
	file_name_pos = strrchr(source, '/');  //��ȡ�ļ���·���е�λ��
	if (file_name_pos == NULL)          //��·��Ϊ�ļ���ʱ��Ĭ�϶Ե�ǰ·�����в���
	{
		strcpy(path, ".");
		strcpy(file_name, source);
		return;
	}
	int path_len = file_name_pos - source;
	for (int i = 0; i < path_len; i++)          //��ȡ·��
		path[i] = source[i];
	path[path_len] = '\0';
	int len = strlen(source);
	int j = 0;
	for (int i = path_len + 1; i < len; i++, j++)  //��ȡ�ļ���
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
	if (from_path_get_dir(path, make_in_dir))      //����·��Ѱ�Ҵ�������Ŀ¼
	{
		if (find_dir_in_dir(make_in_dir, dir_name, pos))   //����Ŀ¼���Ƿ����ͬ��Ŀ¼
		{
			sprintf(print_data, "%s %s Ŀ¼���Ѵ�����Ϊ % s ���ļ����ļ�����ʧ��!\n", print_data, path, dir_name);
			return;
		}
		else            //����Ŀ¼
		{
			long addr = -1;	//����Ŀ¼�����������׵�ַ
			int _inode = -1;	//����Ŀ¼��inode
			int b_index;	//��������ݿ������ݿ�λͼ����ʼλ��
			addr = blocks_allocation(dir_sizeof_block, b_index);   //�������ݿ飨�����ڻ��޸ĳ����죩
			if (addr < 0)
			{
				strcat(print_data, "Ӳ�̿ռ䲻����Ŀ¼����ʧ��!\n");
				return;
			}
			else
			{
				_inode = inode_allocation();
				if (_inode < 0)
				{
					strcat(print_data, "������ʧ�ܣ�Ŀ¼����ʧ�ܣ�\n");
					free_blocks(dir_sizeof_block, b_index);   //�ͷ���ǰ��������ݿ�
					return;
				}
				//Ŀ¼����
				dir create_dir;
				strcpy(create_dir.dir_name, dir_name);
				create_dir.inode = (unsigned int)_inode;
				create_dir.size = 2;
				create_dir.file_entry[0].inode = make_in_dir.inode;
				strcpy(create_dir.file_entry[0].name, "..");
				create_dir.file_entry[1].inode = (unsigned int)_inode;
				strcpy(create_dir.file_entry[1].name, ".");
				//���������Ϣ
				_inode_table[_inode].i_addr = addr;
				_inode_table[_inode].i_blocks = dir_sizeof_block;
				_inode_table[_inode].i_change = false;
				_inode_table[_inode].i_limit = rw;
				_inode_table[_inode].i_model = _directory;
				_inode_table[_inode].i_size = sizeof(dir);
				_inode_table[_inode].i_uid = uid;
				//�޸ĸ�Ŀ¼��Ϣ
				strcpy(make_in_dir.file_entry[make_in_dir.size].name, dir_name);
				make_in_dir.file_entry[make_in_dir.size].inode = (unsigned int)_inode;
				make_in_dir.size += 1;
				if (make_in_dir.inode == current_dir.inode)   //�����ڵ�ǰĿ¼�´���������Ҫ���µ�ǰĿ¼����Ϣ
					current_dir = make_in_dir;
				errno_t err = fopen_s(&fd, "file_control_system", "rb+");
				if (err != 0)
				{
					strcat(print_data, "file_control_system open fail!\n");
					exit(0);
				}
				fseek(fd, 0, SEEK_SET);
				fwrite(&_super_block, superblock_bytes, 1, fd);      //д������
				fseek(fd, sizeof(super_block) + sizeof(bool) * b_index, SEEK_SET);
				fwrite(&block_bitmap[b_index], sizeof(bool), dir_sizeof_block, fd);  //д���ݿ�ͼ
				fseek(fd, sizeof(super_block) + sizeof(bool) * (blocks_num + _inode), SEEK_SET);
				fwrite(&inode_bitmap[_inode], sizeof(bool), 1, fd);            //д���λͼ
				fseek(fd, sizeof(super_block) + sizeof(bool) * (blocks_num + inodes_num) + sizeof(inode) * _inode, SEEK_SET);
				fwrite(&_inode_table[_inode], inode_bytes, 1, fd);   //д�������
				fseek(fd, addr, SEEK_SET);
				fwrite(&create_dir, dir_bytes, 1, fd);              //������Ŀ¼д���ļ�
				fseek(fd, _inode_table[make_in_dir.inode].i_addr, SEEK_SET);
				fwrite(&make_in_dir, dir_bytes, 1, fd);             //����Ŀ¼��д���ļ�
				fclose(fd);
				sprintf(print_data, "%s����%sĿ¼�ɹ�\n", print_data, dir_name);
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
	if (from_path_get_dir(path, remove_in_dir))      //����·��Ѱ��ɾ��Ŀ¼�ĸ�Ŀ¼
	{
		dir rm_dir = remove_in_dir;
		int rm_dir_pos = -1;
		if (!find_dir_in_dir(rm_dir, dir_name, rm_dir_pos))           //����ɾ��Ŀ¼����Ŀ¼�Ƿ����ɾ��Ŀ¼
		{
			sprintf(print_data, "%s%sĿ¼������\n", print_data, dir_name);
			fclose(fd);
			writeSession(SESSION_ADDR, 0);            //֪ͨshell�رջỰ
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
			if (if_dir_in_dir(current_dir, rm_dir))       //ɾ��Ŀ¼�Ƿ������ǰĿ¼
			{
				strcat(print_data, "ɾ��ʧ�ܣ��н�������ʹ��ɾ��Ŀ¼�е��ļ���\n\0");
				fclose(fd);
				writeSession(SESSION_ADDR, 0);
				return;
			}
			if (rm_dir.size > 2)            //Ŀ¼��Ϊ��
			{
				sprintf(print_data, "%s%s ��Ŀ¼�����ļ����Ƿ�Ҫɾ��(y/n)\n\0", print_data, dir_name);
				flushData(DATA_ADDR);
				writeData(DATA_ADDR, print_data);
				writeFlag(FLAG_ADDR, 1);
				char flag;
				while (true)               //�ȴ�shellд���ļ�����
				{
					readFlag(FLAG_ADDR, flag);
					if (flag == 0)break;
				}
				char* buffer1 = new char[100];
				memset(buffer1, 0, 100);
				readData(DATA_ADDR, buffer1);   //�ӹ����ڴ��ж�ȡѡ��
				writeSession(SESSION_ADDR, 0);    //�Ự����
				if (strcmp(buffer1, "y") == 0)
				{
					remove_dir(remove_in_dir, rm_dir);	//�ݹ�ɾ�����ļ�(�ļ���Ŀ¼)
					for (int i = rm_dir_pos; i < remove_in_dir.size - 1; i++)   //�޸ĸ�Ŀ¼
					{
						remove_in_dir.file_entry[i].inode = remove_in_dir.file_entry[i + 1].inode;
						strcpy(remove_in_dir.file_entry[i].name, remove_in_dir.file_entry[i + 1].name);
					}
					remove_in_dir.size--;
					if (remove_in_dir.inode == current_dir.inode)
						current_dir = remove_in_dir;
					fseek(fd, _inode_table[remove_in_dir.inode].i_addr, SEEK_SET);   //����Ŀ¼д��ϵͳ�ļ�
					fwrite(&remove_in_dir, dir_bytes, 1, fd);
					fclose(fd);
					sprintf(print_data, "%sĿ¼ɾ���ɹ�\n", dir_name);        //����ǰ���д������ռ������
					return;
				}
				else
				{
					strcpy(print_data, "Ŀ¼ɾ����ֹ!\n");     //����ǰ���д������ռ������
					fclose(fd);
					return;
				}
			}
			else            //Ŀ¼Ϊ��
			{
				free_blocks(dir_sizeof_block, (_inode_table[rm_dir.inode].i_addr - first_data_addr) / block_size);
				free_inode(rm_dir.inode);      //�ͷŸ�Ŀ¼���ݿ��λͼ�ɹ�
				for (int i = rm_dir_pos; i < remove_in_dir.size - 1; i++)   //�޸ĸ�Ŀ¼
				{
					remove_in_dir.file_entry[i].inode = remove_in_dir.file_entry[i + 1].inode;
					strcpy(remove_in_dir.file_entry[i].name, remove_in_dir.file_entry[i + 1].name);
				}
				remove_in_dir.size--;
				if (remove_in_dir.inode == current_dir.inode)
					current_dir = remove_in_dir;
				fseek(fd, _inode_table[remove_in_dir.inode].i_addr, SEEK_SET);   //����Ŀ¼д��ϵͳ�ļ�
				fwrite(&remove_in_dir, dir_bytes, 1, fd);
				fclose(fd);
				writeSession(SESSION_ADDR, 0);    //�Ự����
				sprintf(print_data, "%s%sĿ¼ɾ���ɹ�\n", print_data, dir_name);
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
	for (i = 2; i < rm_dir.size; i++)          //ѭ��ɾ�����ļ�
	{
		if (_inode_table[rm_dir.file_entry[i].inode].i_model == _directory)    //Ŀ¼
		{
			dir now_dir;
			fseek(fd, _inode_table[rm_dir.file_entry[i].inode].i_addr, SEEK_SET);
			fread(&now_dir, dir_bytes, 1, fd);
			remove_dir(rm_dir, now_dir);
		}
		else
			delete_file(rm_dir, i);             //�ļ�
	}
	free_blocks(dir_sizeof_block, (_inode_table[rm_dir.inode].i_addr - first_data_addr) / block_size);
	free_inode(rm_dir.inode);      //�ͷ����ݿ��λͼ�ɹ�
}