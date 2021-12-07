#ifndef MEMORY_H
#define MEMORY_H

#include<Windows.h>

extern LPVOID pBuffer;
extern HANDLE hMapFile;
//�ڴ��С
#define FLAG_SIZE            sizeof(char)     //��־λ��0��ʾǰ�˿�д��1��ʾ��˿�д��ÿ�ο�дǰ�ٶ�����
#define SESSION_SIZE         sizeof(char)        //�Ự��־λ
#define MAX_PATH_SIZE         1024
#define MAX_BUF_SIZE           1024*1024*10    //ͨ�ŵ���Ϣ�����ɴ�10M��

#define MEMORY_SIZE        FLAG_SIZE + SESSION_SIZE + MAX_PATH_SIZE + MAX_BUF_SIZE

//�ڴ����Ϣ��ַ
#define FLAG_ADDR           (char*)pBuffer
#define SESSION_ADDR        FLAG_ADDR + FLAG_SIZE
#define CURRENT_PATH_ADDR   SESSION_ADDR + SESSION_SIZE
#define DATA_ADDR           CURRENT_PATH_ADDR + MAX_PATH_SIZE

//���ܺ���
bool initShareMemory();
void readFlag(char* Flag_addr, char& value);
void writeFlag(char* Flag_addr, char value);
void writeSession(char* Session_addr, char value);
void readSession(char* Session_addr, char& value);
void writePath(char* Path_addr, char* path);
void writeData(char* Data_addr, char* buf);
void readData(char* Data_addr, char* buf);
void flushData(char* Data_addr);



#endif