#include"login.h"

bool login()
{
	strcat(print_data, "������������������������������������������������������������������������������-\n");
	strcat(print_data, "|                       �༶: 19�ƿ�2��                                       |\n");
	strcat(print_data, "|                       ����: ���                                          |\n");
	strcat(print_data, "|                       ѧ�ţ�201930341496                                    |\n");
	strcat(print_data, "������������������������������������������������������������������������������-\n");
	int id;
	char Password[100];
	printf("�û�: ");
	fflush(stdin);
	scanf("%dl", &id);
	printf("\n����: ");
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