#ifndef MEMORY_H
#define MEMORY_H

#include<Windows.h>

extern LPVOID pBuffer;
extern HANDLE hMapFile;
extern struct order _order[13];
//�ڴ��С
#define FLAG_SIZE          sizeof(char)     //��־λ��0��ʾǰ�˿�д��1��ʾ��˿�д��ÿ�ο�дǰ�ٶ�����
#define MAX_PATH_SIZE         1024
#define MAX_BUF_SIZE           1024*1024*10    //ͨ�ŵ���Ϣ�����ɴ�10M��

#define MEMORY_SIZE        FLAG_SIZE + MAX_PATH_SIZE + MAX_BUF_SIZE

//�ڴ����Ϣ��ַ
#define FLAG_ADDR           (char*)pBuffer
#define CURRENT_PATH_ADDR   FLAG_ADDR + FLAG_SIZE
#define DATA_ADDR           CURRENT_PATH_ADDR + MAX_PATH_SIZE

//���ܺ���
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