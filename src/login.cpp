#include"login.h"

bool login()
{
	strcat(print_data, "！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！-\n");
	strcat(print_data, "|                       萎雫: 19柴親2萎                                       |\n");
	strcat(print_data, "|                       侖兆: 川伸巖                                          |\n");
	strcat(print_data, "|                       僥催��201930341496                                    |\n");
	strcat(print_data, "！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！-\n");
	int id;
	char Password[100];
	printf("喘薩: ");
	fflush(stdin);
	scanf("%dl", &id);
	printf("\n畜鷹: ");
	fflush(stdin);
	scanf("%s", Password);
	fflush(stdin);
	printf("\n");
	for (int i = 0; i < max_user_num; i++)
	{
		if (id == _user[i].uid && strcmp(Password, _user[i].password) == 0)
		{
			uid = id;
			return true;
		}
	}
	return false;
}