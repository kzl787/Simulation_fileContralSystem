#include"structure.h"
#include<stdio.h>
#include<stdlib.h>

/*
 *����:�������ݿ�ռ�
 *
 *�㷨����:1.���ʣ����п��Ƿ�������䣬������ת2����������ת4��
 *        2.����Ƿ�����������������������飬������ת3������װ4
 *        3.�����������ݿ���Ϊ�ѱ�ʹ�ã��޸����ݿ�λͼ�������顢���ط�����׵�ַ
 *        4.����-1
 *����˵����size:�ļ���Ҫ���̿���    BlockAddr_index���ļ������ʼ�̿��
 */
long blocks_allocation(int size, int& BlockAddr_index);

/*
 ���ܣ������㣬���ؽ���
*/
int inode_allocation();