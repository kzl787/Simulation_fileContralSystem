#define _CRT_SECURE_NO_WARNINGS
#include"help.h"
/*
 ���ܣ���ӡ�����ʽ
*/
void help()
{
	strcat(print_data, "\n�����������Ϊ����,�ұ�Ϊ�����ʽ,����ʱ�밴��ʽ����!\n");
	strcat(print_data, "1.·��֧�־���·�������·��,��ǰĿ¼Ϊ . ��һ��Ŀ¼Ϊ .. \n");
	strcat(print_data, "3.����host�ļ�ϵͳ������ļ�����ʱ·����ǰӦ��<host>,��·������\\����\n\n");
	strcat(print_data, "0.��ʼ��---------------format\n");
	strcat(print_data, "1.��ʾϵͳ��Ϣ---------info\n");
	strcat(print_data, "2.�ı�Ŀ¼-------------cd + �ո� + ·��\n");
	strcat(print_data, "3.��ʾĿ¼-------------dir + �ո� + ·��\n");
	strcat(print_data, "4.����Ŀ¼-------------md + �ո� + ·��(����Ŀ¼��)\n");
	strcat(print_data, "5.ɾ��Ŀ¼-------------rd + �ո� + ·��(����Ŀ¼��)\n");
	strcat(print_data, "6.�����ļ�-------------newfile + �ո� + ·��(�����ļ���)\n");
	strcat(print_data, "7.���ļ�-------------cat + �ո� + ·��(�����ļ���)\n");
	strcat(print_data, "8.�����ļ�-------------copy + �ո� + Դ·�� + �ո� + Ŀ��·��\n");
	strcat(print_data, "9.ɾ���ļ�-------------del + �ո� +·��(�����ļ���)\n");
	strcat(print_data, "10.��Ⲣ�ָ��ļ�ϵͳ--check\n");
	strcat(print_data, "11.�˳��ļ�ģ��--------exit\n");
	strcat(print_data, "12.����----------------help\n\n");
}