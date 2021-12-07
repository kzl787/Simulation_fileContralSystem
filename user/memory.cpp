#define _CRT_SECURE_NO_WARNINGS
#include"memory.h"
#include<string>
#include<Windows.h>
#include<tchar.h>


using namespace std;

HANDLE hMapFile;
LPVOID pBuffer;
TCHAR memeryName[] = TEXT("SharePicture");
struct order _order[13];

bool initShareMemory()
{

	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		MEMORY_SIZE,
		memeryName
	);

	if (hMapFile == NULL)
	{
		return false;
	}

	pBuffer = (LPTSTR)MapViewOfFile(
		hMapFile,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		MEMORY_SIZE
	);

	if (pBuffer == NULL)
	{
		CloseHandle(hMapFile);
		return false;
	}
	//初始化共享内存内容全是0
	memset((char*)pBuffer, 0, MEMORY_SIZE);
	//初始化命令
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
	return true;
}

void readFlag(char* Flag_addr, char& value)
{
	memcpy(&value, Flag_addr, sizeof(value));
}


void writeFlag(char* Flag_addr, char value)
{
	memcpy(Flag_addr, &value, sizeof(value));
}

void writePath(char* Path_addr, char* path)
{
	int size = strlen(path) + 1;
	memcpy(Path_addr, path, size);
}

void writeData(char* Data_addr, char* buf)
{
	int size = strlen(buf) + 1;
	memcpy(Data_addr, buf, size);
}


void readData(char* Data_addr, char* buf)
{
	int size = strlen(Data_addr) + 1;
	memcpy(buf, Data_addr, size);
}