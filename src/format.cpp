#define _CRT_SECURE_NO_WARNINGS
#include"format.h"


void format()
{
	strcat(print_data,"��ʼ��ϵͳ�ļ��У�\0");

	long system_size = first_data_addr + blocks_num * block_size;    //ϵͳ��ȫ����С
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

	//��ʼ��������
	_super_block.s_blocks_size = block_size;
	_super_block.s_blocks_count = blocks_num;
	_super_block.s_inodes_count = inodes_num;
	_super_block.s_first_data_addr = first_data_addr;
	_super_block.s_free_blocks_count = blocks_num;
	_super_block.s_free_inodes_count = inodes_num;

	//��ʼ������λͼ
	int i = 0;
	for (i = 0; i < inodes_num; i++)
		inode_bitmap[i] = not_use;
	for (i = 0; i < blocks_num; i++)
		block_bitmap[i] = not_use;

	//��ʼ�������
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

	/*������Ŀ¼*/
	//��ʼ����Ŀ¼��inode
	_inode_table[0].i_blocks = dir_sizeof_block;
	_inode_table[0].i_addr = first_data_addr;
	_inode_table[0].i_change = false;
	_inode_table[0].i_model = _directory;
	_inode_table[0].i_size = sizeof(dir);
	_inode_table[0].i_uid = 341496;
	_inode_table[0].i_limit = read_only;
	//���ĳ����졢λͼ����Ϣ
	_super_block.s_free_blocks_count -= dir_sizeof_block;
	_super_block.s_free_inodes_count -= 1;
	inode_bitmap[0] = used;
	for (i = 0; i < dir_sizeof_block; i++)
		block_bitmap[i] = used;
	//��ʼ����Ŀ¼����Ϣ
	strcpy(current_dir.dir_name, "root");         //Ŀ¼��
	current_dir.inode = 0;                      //��Ŀ¼���������
	current_dir.size = 2;                       //�ļ�����
	strcpy(current_dir.file_entry[0].name, "..");  //��Ŀ¼
	current_dir.file_entry[0].inode = 0;          //�Ǹ�Ŀ¼
	strcpy(current_dir.file_entry[1].name, ".");  //��ǰĿ¼
	current_dir.file_entry[1].inode = 0;          //Ҳ�Ǹ�Ŀ¼

	//��ʼ���������ṹ��
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

	//��ϵͳ��ʼ����Ϣд���������
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
		char select = 's';	//��ʼ��Ϊs��������
		fflush(stdin);		//�����
		do
		{
			if (select != '\n')
				strcat(print_data, "ģ��Linuxϵͳ�ļ������ڣ��Ƿ���и�ʽ��(y/n): \n");
			select = getchar();              //СС��bug����Ϊ�ļ�ҽ���ڣ����Ծ��岻��ִ��
			if (select == 'y' || select == 'Y' || select == 'n' || select == 'N')
			{
				fflush(stdin);		//�����
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
			strcat(print_data, "ģ��Linux�ļ�ϵͳ��ʧ��!\n");
			exit(0);
		}
	}

	//����ϵͳ��Ϣ
	fseek(fd, 0, SEEK_SET);
	fread(&_super_block, superblock_bytes, 1, fd);
	fread(&block_bitmap, sizeof(bool), blocks_num, fd);
	fread(&inode_bitmap, sizeof(bool), inodes_num, fd);
	fread(&_inode_table, inode_bytes, inodes_num, fd);
	fread(&current_dir, dir_bytes, 1, fd);
	fclose(fd);

	//��ʼ������Ա
	_user[0].model = ADMIN; 
	_user[0].uid = 341496;
	strcpy(_user[0].password, "123456");
	//����ϵͳ�û�
	_user[1].model = CASUAL;
	_user[1].uid = 740186;
	strcpy(_user[1].password, "123456");

	//��ʼ���������ṹ��
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

	//��ʼ��·��
	strcpy(current_path, "root>");
}