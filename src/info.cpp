#define _CRT_SECURE_NO_WARNINGS
#include"info.h"

void info()
{
	sprintf(print_data, "%s\n%15s\n", print_data, "ģ��Linux�ļ�ϵͳ��Ϣ:");
	sprintf(print_data, "%s%15s  %10d %-3s\n", print_data, "�̿�Ĵ�С:", _super_block.s_blocks_size, "�ֽ�");
	sprintf(print_data, "%s%15s  %10d %-3s\n", print_data, "�ܵ��̿���:", _super_block.s_blocks_count, "��");
	sprintf(print_data, "%s%15s  %10d %-3s\n", print_data, "���п������:", _super_block.s_free_blocks_count, "��");
	sprintf(print_data, "%s%15s  %10d %-3s\n", print_data, "������Ϊ:", _super_block.s_blocks_count * block_size, "�ֽ�");
	sprintf(print_data, "%s%15s  %10d %-3s\n", print_data, "ʣ��ռ�Ϊ:", _super_block.s_free_blocks_count * block_size, "�ֽ�");
}