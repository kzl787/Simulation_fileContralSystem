#define _CRT_SECURE_NO_WARNINGS
#include"memory.h"
#include<string>
#include<Windows.h>
#include<tchar.h>

using namespace std;

HANDLE hMapFile;
LPVOID pBuffer;
TCHAR memeryName[] = TEXT("SharePicture");

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

void writeSession(char* Session_addr, char value)
{
	memcpy(Session_addr, &value, sizeof(value));
}

void readSession(char* Session_addr, char& value)
{
	memcpy(&value, Session_addr, sizeof(value));
}

void writePath(char* Path_addr, char* path)
{
	int size = strlen(path);
	memcpy(Path_addr, path, size);
}

void writeData(char* Data_addr, char* buf)
{
	int size = strlen(buf);
	memcpy(Data_addr, buf, size);
}


void readData(char* Data_addr, char* buf)
{
	int size = strlen(buf);
	strcpy(buf, Data_addr);
}

void flushData(char* Data_addr)
{
	memset(Data_addr, 0, MAX_BUF_SIZE);
}