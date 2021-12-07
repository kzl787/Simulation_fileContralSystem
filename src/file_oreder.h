#include"structure.h"
#include"dir_order.h"
#include"free_space.h"
#include"allocation.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*
*���ݸ�����Ŀ¼���ļ��������Ҹ�Ŀ¼�Ƿ��и��ļ��������򷵻���
*/
bool find_file_in_dir(dir temp, char* name, int &pos);

/*
 ���ܣ�ɾ���ļ���
 ������path_and_filename��·�����ļ������ַ�����
 �㷨���裺��ȫ·�������·����ɾ�����ļ�����
		 ·����ȷ�Ҵ��ڸ��ļ��������delete_file����ɾ����
		 ������ʾ����
*/
void del_file(char* path_and_filename);


/*
 ���ܣ�ɾ��ָ��Ŀ¼�е��ļ���ͬʱ���޸Ĺ���λͼ�����ݿ�ͼ��������д��ϵͳ�ļ�������ǰ���ϵͳ�ļ���
 �������ͣ�remove_dir:Ҫɾ�����ļ����ڵ�Ŀ¼��  file_pos���ļ���Ŀ¼.file_entry�е�λ��
*/
void delete_file(dir remove_in_dir, int file_pos);


/*
 ���ܣ��½��ļ�
 ������path_and_filename��·�����ļ������ַ�����
 �㷨���裺��ȫ·�������·����ɾ�����ļ�����
		 ·����ȷ��
		 ���Ŀ¼���Ƿ����ͬ���ļ���������ʾ������Ϣ��
		 ���������ļ����ݣ�ֱ������س����������ݿ�ͽ�㣬���޸�ϵͳ�ļ���Ϣ��������ʾ�ռ����ʧ�ܣ�
*/

void create_file(char* path_and_filename);

/*
 ���ܣ����ļ�
 path_and_filename��·�����ļ������ַ�����
 �㷨����ȫ·�������·����ɾ�����ļ�����
		 ·����ȷ����Ŀ¼���Ƿ���ڸ��ļ���������ʾ������Ϣ��
		 Ȼ��Ӵ�������ȡ���ļ�������������󽫻����������
*/
void open_file(char* path_and_filename);

/*
 ���ܣ������ļ���
 ʵ�ֲ��裺
*/
void copy_file(char* source_path, char* des_path);

/*
 ���ܣ��ж�·���Ƿ�Ϊ�ⲿ������
 �������������·�����Ƿ����<host>�ַ����������򷵻�ϵͳ·����true�����򷵻�false��
*/
bool enviroment(char* path);

/*
 ���ܣ��½�һ���ļ�������������д�����ļ��У�
 ������temp���ļ�����Ŀ¼��
*/
void write_file(dir& temp, char* name, char* buf, long flen);