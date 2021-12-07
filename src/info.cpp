#define _CRT_SECURE_NO_WARNINGS
#include"info.h"

void info()
{
	sprintf(print_data, "%s\n%15s\n", print_data, "模拟Linux文件系统信息:");
	sprintf(print_data, "%s%15s  %10d %-3s\n", print_data, "盘块的大小:", _super_block.s_blocks_size, "字节");
	sprintf(print_data, "%s%15s  %10d %-3s\n", print_data, "总的盘块数:", _super_block.s_blocks_count, "块");
	sprintf(print_data, "%s%15s  %10d %-3s\n", print_data, "空闲块的总数:", _super_block.s_free_blocks_count, "块");
	sprintf(print_data, "%s%15s  %10d %-3s\n", print_data, "总容量为:", _super_block.s_blocks_count * block_size, "字节");
	sprintf(print_data, "%s%15s  %10d %-3s\n", print_data, "剩余空间为:", _super_block.s_free_blocks_count * block_size, "字节");
}