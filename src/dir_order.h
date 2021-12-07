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
关于对目录文件的一系列操作
*/

/*
 *功能：根据给定的目录和文件名，查找该目录是否含有该目录，存在则更新目录到目标目录中（传递过来的参数）(调用前需先打开文件）
*/
bool find_dir_in_dir(dir& temp, char* name, int& file_pos);

/*
*功能：从给出的路径中分析提取到指定目录，如果找到路径的目标文件，则返回该目录
参数说明：参数一：文件路径     参数二：路径最后的目录内容
*/
bool from_path_get_dir(char* path_and_dir_name, dir& dir_buf);

/*
*功能：改变目录
*算法步骤：
	判断路径是否存在；
	存在则改变当前目录为目标目录；
	若不存在，则提示出错信息；
参数说明：要改变到目标目录的路径
*/
void change_dir(char* path_and_dirname);

/*
 *功能:提取指定目录的路径(调用前需先打开系统文件）
 *
 *算法步骤:递归查找上一级目录，直到找到根目录为止
 *参数说明：temp：提取目标目录路径
 */
void get_dir_path(dir temp);


/*
*功能：显示目录下的文件及子目录
*算法步骤：
	根据目录的大小，循环显示子目录和文件的相关信息
*/
void display(dir temp);

/*
*功能：输出文件权限
*/
void limit(int lim);

/*
 *功能:显示目录,显示当前目录或指定目录(绝对路径或相对路径)
 *
 *算法步骤:
 *		   1.判断是否为显示当前目录，是则转4，否则转2
 *		   2.判断是否从根目录开始，是则从根目录开始查找到指定目录的inode，转4，否则转3
 *         3.判断是否为当前目录下的子目录，是则找到指定目录的inode，转4，否则表示无该目
 *           录，返回出错信息
 *         4.循环显示文件和目录
 */
bool display_dir(char* path);

/*
 *功能:提取出路径和文件名
 *参数说明：source_path：全路径；  path：提取出的路径；  file_name:提取出的文件名
 */
void divide_PathAndName(char* source, char* path, char* file_name);


/*
 *功能:创建目录
 *
 *算法步骤:分析给的路径，分解出要创建的目录的路径和要创建的目录名；
 *        找到要创建目录所在目录，
 *		  然后判断该目录下是否有与要创建的目录的同名目录，有则给出提示信息，否则分配空间创建目录
 */
void creat_dir(char* path_and_dirname);


/*
功能：判读目标目录是否在源目录中(调用前需先打开系统文件流）
参数：destination：目标目录； source：源目录
*/
bool if_dir_in_dir(dir destination, dir source);

/*
功能：删除目录；
算法实现：分析给出的路径，分解出路径和要删除的目录名；
		找到删除目录所在的目录；
		判断目录是否为空，不为空则询问用户意见；
		用户选择删除则判断当前目录是否在删除的目录中；
		是则不能删除；
		循环删目录下的文件；
*/
void remove_dir(char* path_and_dirname);

/*
 功能：循环删除目录及其下文件（调用前需打开系统文件流）
 参数：father_dir：’删除目录‘所在的目录， remove_dir：要删除的目录；
*/
void remove_dir(dir father_dir, dir rm_dir);

#endif