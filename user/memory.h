#ifndef MEMORY_H
#define MEMORY_H

#include<Windows.h>

extern LPVOID pBuffer;
extern HANDLE hMapFile;
extern struct order _order[13];
//内存大小
#define FLAG_SIZE          sizeof(char)     //标志位（0表示前端可写，1表示后端可写）每次可写前再读内容
#define MAX_PATH_SIZE         1024
#define MAX_BUF_SIZE           1024*1024*10    //通信的信息量最大可达10M的

#define MEMORY_SIZE        FLAG_SIZE + MAX_PATH_SIZE + MAX_BUF_SIZE

//内存各信息地址
#define FLAG_ADDR           (char*)pBuffer
#define CURRENT_PATH_ADDR   FLAG_ADDR + FLAG_SIZE
#define DATA_ADDR           CURRENT_PATH_ADDR + MAX_PATH_SIZE

//功能函数
bool initShareMemory();
void readFlag(char* Flag_addr, char& value);
void writeFlag(char* Flag_addr, char value);
void writePath(char* Path_addr, char* path);
void writeData(char* Data_addr, char* buf);
void readData(char* Data_addr, char* buf);

struct order {
	char oreder_name[10];
};


#endif