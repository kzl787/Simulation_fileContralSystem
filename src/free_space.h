#include"structure.h"
#include<stdio.h>
#include<stdlib.h>
/*
*���ܣ��ͷ����ݿ飨����ǰ���ϵͳ�ļ��� �����лὫ�޸����λͼ,�Լ�������д��ϵͳ��
*����˵���� count���ͷ����ݿ�������� b_index���ͷŵĵ�һ�����ݿ��
*/
bool free_blocks(int count, int b_index);

/*
 ���ܣ��ͷŽ�㣻(����ǰ��Ҫ��ϵͳ�ļ��������Ὣ�޸����λͼ�ͳ�����д��ϵͳ)
*/
bool free_inode(int i_index);