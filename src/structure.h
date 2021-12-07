#ifndef STRUCTURE_H
#define STRUCTURE_H
#include<stdio.h>
#include<stdlib.h>

#define read_only            1          //ֻ��
#define write_only           2          //ֻд
#define rw                   3          //��д

#define ADMIN				1		//ϵͳ����Ա
#define CASUAL				0		//��ͨ�û�

#define not_use              false      //δʹ��
#define used                  true       //�ѱ�ʹ��

#define _file                1          //�ļ�
#define _directory           2          //Ŀ¼

#define block_size           1024       //�̿��С��1KB
#define blocks_num            (1024*100)  //�̿������
#define inodes_num            (1024*100)  //�ڵ���

#define max_file_name_len    128        //�ļ�����󳤶�
#define max_file_num         256        //Ŀ¼�ļ����ļ����������
#define max_user_num         8          //ϵͳ������û���

#define first_data_addr      (sizeof(super_block) + sizeof(bool)*(inodes_num + blocks_num) + sizeof(inode)*inodes_num)  //��һ�����ݿ�������ַ
#define dir_sizeof_block     (sizeof(dir)/block_size + 1)     //ÿ��Ŀ¼�ļ���ռ���̿������ڲ�ȡ�����洢�ķ�ʽ����Ϊÿ��Ŀ¼�ļ����������洢�ռ�
#define inode_bytes          (sizeof(inode))               //ÿ�������ռ�Ŀռ��С
#define superblock_bytes     (sizeof(super_block))                 //��������ռ�Ŀռ�
#define dir_bytes            (sizeof(dir))                       //Ŀ¼�ļ��Ŀռ��С

/*
*������
*/
struct super_block {
	unsigned int   s_blocks_size;               //ϵͳ�̿�Ĵ�С
	unsigned int   s_blocks_count;              //ϵͳ���̿���
	unsigned int   s_inodes_count;              //ϵͳ�ܽڵ���
	unsigned int   s_free_blocks_count;         //ϵͳ�����̿���
	unsigned int   s_free_inodes_count;         //ϵͳ���нڵ���
	long           s_first_data_addr;           //ϵͳ��һ�����ݿ�������ַ
};

/*
*�����ڵ�
*/
struct inode {
	unsigned short i_model;               //�ļ�����
	unsigned int i_uid;                   //�������û�id
	unsigned int i_size;                  //�ļ���С
	unsigned int i_blocks;                //�ļ���ռ�̿���
	unsigned int i_change;                //�ļ��Ƿ����޸�
	long i_addr;                          //�ļ��Ĵ�ŵ������ַ
	unsigned int i_limit;                 //�ļ��Ķ�ȡȨ��
};

/*
*�ļ����ȿ�������ͨ�ļ���Ҳ������Ŀ¼�ļ�
*/
struct File_Entry {
	char name[max_file_name_len];
	unsigned int inode;
};

/*
*Ŀ¼�����ڴ洢����Ŀ¼���������ļ�(����Ŀ¼)����Ŀ�;������ļ������ԣ���file_entry)
*/
struct dir {
	char dir_name[max_file_name_len];        //�ļ���
	unsigned int inode;                      //�������
	unsigned short size;                     //���ļ�����Ŀ
	struct File_Entry file_entry[max_file_num];   //���ļ��飺[�ļ����ƣ� �ļ����������]  Ĭ�ϵ�һλ��Ϊ��Ŀ¼�� �ڶ�λ��Ϊ��ǰĿ¼
};

/*
 *�û�
 */
struct user
{
	unsigned int model;					//�û���ģʽ(Ȩ��)
	int uid;							//�û���ID
	char password[20];					//����
};

struct order {
	char oreder_name[10];
};

extern struct super_block _super_block;
extern bool block_bitmap[blocks_num];
extern bool inode_bitmap[inodes_num];
extern struct inode _inode_table[inodes_num];
extern struct dir current_dir;
extern FILE* fd;
extern unsigned int uid;
extern char current_path[500];
extern struct user _user[max_user_num];
extern struct order _order[13];
extern char* print_data;






#endif
