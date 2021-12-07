#ifndef DIR_ORDER_H
#define DIR_ORDER_H

#include"structure.h"
#include"free_space.h"
#include"allocation.h"
#include"file_oreder.h"
#include"memory.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*
���ڶ�Ŀ¼�ļ���һϵ�в���
*/

/*
 *���ܣ����ݸ�����Ŀ¼���ļ��������Ҹ�Ŀ¼�Ƿ��и�Ŀ¼�����������Ŀ¼��Ŀ��Ŀ¼�У����ݹ����Ĳ�����(����ǰ���ȴ��ļ���
*/
bool find_dir_in_dir(dir& temp, char* name, int& file_pos);

/*
*���ܣ��Ӹ�����·���з�����ȡ��ָ��Ŀ¼������ҵ�·����Ŀ���ļ����򷵻ظ�Ŀ¼
����˵��������һ���ļ�·��     ��������·������Ŀ¼����
*/
bool from_path_get_dir(char* path_and_dir_name, dir& dir_buf);

/*
*���ܣ��ı�Ŀ¼
*�㷨���裺
	�ж�·���Ƿ���ڣ�
	������ı䵱ǰĿ¼ΪĿ��Ŀ¼��
	�������ڣ�����ʾ������Ϣ��
����˵����Ҫ�ı䵽Ŀ��Ŀ¼��·��
*/
void change_dir(char* path_and_dirname);

/*
 *����:��ȡָ��Ŀ¼��·��(����ǰ���ȴ�ϵͳ�ļ���
 *
 *�㷨����:�ݹ������һ��Ŀ¼��ֱ���ҵ���Ŀ¼Ϊֹ
 *����˵����temp����ȡĿ��Ŀ¼·��
 */
void get_dir_path(dir temp);


/*
*���ܣ���ʾĿ¼�µ��ļ�����Ŀ¼
*�㷨���裺
	����Ŀ¼�Ĵ�С��ѭ����ʾ��Ŀ¼���ļ��������Ϣ
*/
void display(dir temp);

/*
*���ܣ�����ļ�Ȩ��
*/
void limit(int lim);

/*
 *����:��ʾĿ¼,��ʾ��ǰĿ¼��ָ��Ŀ¼(����·�������·��)
 *
 *�㷨����:
 *		   1.�ж��Ƿ�Ϊ��ʾ��ǰĿ¼������ת4������ת2
 *		   2.�ж��Ƿ�Ӹ�Ŀ¼��ʼ������Ӹ�Ŀ¼��ʼ���ҵ�ָ��Ŀ¼��inode��ת4������ת3
 *         3.�ж��Ƿ�Ϊ��ǰĿ¼�µ���Ŀ¼�������ҵ�ָ��Ŀ¼��inode��ת4�������ʾ�޸�Ŀ
 *           ¼�����س�����Ϣ
 *         4.ѭ����ʾ�ļ���Ŀ¼
 */
bool display_dir(char* path);

/*
 *����:��ȡ��·�����ļ���
 *����˵����source_path��ȫ·����  path����ȡ����·����  file_name:��ȡ�����ļ���
 */
void divide_PathAndName(char* source, char* path, char* file_name);


/*
 *����:����Ŀ¼
 *
 *�㷨����:��������·�����ֽ��Ҫ������Ŀ¼��·����Ҫ������Ŀ¼����
 *        �ҵ�Ҫ����Ŀ¼����Ŀ¼��
 *		  Ȼ���жϸ�Ŀ¼���Ƿ�����Ҫ������Ŀ¼��ͬ��Ŀ¼�����������ʾ��Ϣ���������ռ䴴��Ŀ¼
 */
void creat_dir(char* path_and_dirname);


/*
���ܣ��ж�Ŀ��Ŀ¼�Ƿ���ԴĿ¼��(����ǰ���ȴ�ϵͳ�ļ�����
������destination��Ŀ��Ŀ¼�� source��ԴĿ¼
*/
bool if_dir_in_dir(dir destination, dir source);

/*
���ܣ�ɾ��Ŀ¼��
�㷨ʵ�֣�����������·�����ֽ��·����Ҫɾ����Ŀ¼����
		�ҵ�ɾ��Ŀ¼���ڵ�Ŀ¼��
		�ж�Ŀ¼�Ƿ�Ϊ�գ���Ϊ����ѯ���û������
		�û�ѡ��ɾ�����жϵ�ǰĿ¼�Ƿ���ɾ����Ŀ¼�У�
		������ɾ����
		ѭ��ɾĿ¼�µ��ļ���
*/
void remove_dir(char* path_and_dirname);

/*
 ���ܣ�ѭ��ɾ��Ŀ¼�������ļ�������ǰ���ϵͳ�ļ�����
 ������father_dir����ɾ��Ŀ¼�����ڵ�Ŀ¼�� remove_dir��Ҫɾ����Ŀ¼��
*/
void remove_dir(dir father_dir, dir rm_dir);

#endif