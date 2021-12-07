#include"structure.h"
#include"dir_order.h"
#include"free_space.h"
#include"allocation.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*
*根据给定的目录和文件名，查找该目录是否含有该文件，存在则返回真
*/
bool find_file_in_dir(dir temp, char* name, int &pos);

/*
 功能：删除文件；
 参数：path_and_filename：路径和文件名的字符串；
 算法步骤：将全路径分离成路径和删除的文件名；
		 路径正确且存在改文件，则调用delete_file进行删除；
		 否则提示错误；
*/
void del_file(char* path_and_filename);


/*
 功能：删除指定目录中的文件，同时将修改过的位图，数据块图，超级快写回系统文件（调用前需打开系统文件）
 参数解释：remove_dir:要删除的文件所在的目录；  file_pos：文件在目录.file_entry中的位置
*/
void delete_file(dir remove_in_dir, int file_pos);


/*
 功能：新建文件
 参数：path_and_filename：路径和文件名的字符串；
 算法步骤：将全路径分离成路径和删除的文件名；
		 路径正确；
		 检查目录中是否存在同名文件，是则提示错误信息；
		 否则输入文件内容，直到输入回车，分配数据块和结点，则修改系统文件信息，否则提示空间分配失败；
*/

void create_file(char* path_and_filename);

/*
 功能：打开文件
 path_and_filename：路径和文件名的字符串；
 算法：将全路径分离成路径和删除的文件名；
		 路径正确则检查目录中是否存在该文件，否则提示错误信息；
		 然后从磁盘中提取出文件到缓冲区，最后将缓冲区输出；
*/
void open_file(char* path_and_filename);

/*
 功能：复制文件；
 实现步骤：
*/
void copy_file(char* source_path, char* des_path);

/*
 功能：判断路径是否为外部环境；
 过程描述：检查路径中是否存在<host>字符串，存在则返回系统路径和true。否则返回false；
*/
bool enviroment(char* path);

/*
 功能：新建一个文件将缓冲区内容写到该文件中；
 参数：temp：文件所在目录，
*/
void write_file(dir& temp, char* name, char* buf, long flen);