#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#pragma warning(disable:4996)

int main()
{
	char key;
	char f_key;
	puts("\t����������");
	puts("\tApplicaation");
	puts("\t202207022");
	puts("================================");

	while (1)
	{
		printf("command>");
		key = _getche();
		if (key == 27)
		{
			printf("\n ���α׷��� ����˴ϴ�.");
			break;
		}

		while (1) {
			f_key = _getch();
			if ((f_key) == '\r') break;
		}

		printf("\n");
		if (key == 'q') puts("qwer ȭ����");
		else if (key == 'w') puts("���� �ʹ� ����");
		else if (key == 'e') puts("���ֺ��� ������ 3���");
		else if (key == 'r') puts("�ູ�մϴ�");
		else puts("�� �� �Է��ϼ̽��ϴ�.");
	}

}