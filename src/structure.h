#ifndef STRUCTURE_H
#define STRUCTURE_H
#include<stdio.h>
#include<stdlib.h>

#define read_only            1          //只读
#define write_only           2          //只写
#define rw                   3          //读写

#define ADMIN				1		//系统管理员
#define CASUAL				0		//普通用户

#define not_use              false      //未使用
#define used                  true       //已被使用

#define _file                1          //文件
#define _directory           2          //目录

#define block_size           1024       //盘块大小：1KB
#define blocks_num            (1024*100)  //盘块的数量
#define inodes_num            (1024*100)  //节点数

#define max_file_name_len    128        //文件名最大长度
#define max_file_num         256        //目录文件中文件的最大数量
#define max_user_num         8          //系统的最多用户数

#define first_data_addr      (sizeof(super_block) + sizeof(bool)*(inodes_num + blocks_num) + sizeof(inode)*inodes_num)  //第一块数据块的物理地址
#define dir_sizeof_block     (sizeof(dir)/block_size + 1)     //每个目录文件所占的盘快数由于采取连续存储的方式所以为每个目录文件分配了最大存储空间
#define inode_bytes          (sizeof(inode))               //每个结点所占的空间大小
#define superblock_bytes     (sizeof(super_block))                 //超级块所占的空间
#define dir_bytes            (sizeof(dir))                       //目录文件的空间大小

/*
*超级块
*/
struct super_block {
	unsigned int   s_blocks_size;               //系统盘块的大小
	unsigned int   s_blocks_count;              //系统总盘口数
	unsigned int   s_inodes_count;              //系统总节点数
	unsigned int   s_free_blocks_count;         //系统空闲盘块数
	unsigned int   s_free_inodes_count;         //系统空闲节点数
	long           s_first_data_addr;           //系统第一个数据块的物理地址
};

/*
*索引节点
*/
struct inode {
	unsigned short i_model;               //文件类型
	unsigned int i_uid;                   //创建的用户id
	unsigned int i_size;                  //文件大小
	unsigned int i_blocks;                //文件所占盘块数
	unsigned int i_change;                //文件是否发生修改
	long i_addr;                          //文件的存放的物理地址
	unsigned int i_limit;                 //文件的读取权限
};

/*
*文件：既可以是普通文件，也可以是目录文件
*/
struct File_Entry {
	char name[max_file_name_len];
	unsigned int inode;
};

/*
*目录：用于存储具体目录中所包含文件(包括目录)的数目和具体子文件的属性（即file_entry)
*/
struct dir {
	char dir_name[max_file_name_len];        //文件名
	unsigned int inode;                      //索引结点
	unsigned short size;                     //子文件的数目
	struct File_Entry file_entry[max_file_num];   //子文件组：[文件名称， 文件的索引结点]  默认第一位置为父目录， 第二位置为当前目录
};

/*
 *用户
 */
struct user
{
	unsigned int model;					//用户的模式(权限)
	int uid;							//用户的ID
	char password[20];					//密码
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
