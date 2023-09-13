#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#pragma warning(disable:4996)

int main()
{
	char key;
	char f_key;
	puts("\t노종현빈의");
	puts("\tApplicaation");
	puts("\t202207022");
	puts("================================");

	while (1)
	{
		printf("command>");
		key = _getche();
		if (key == 27)
		{
			printf("\n 프로그램이 종료됩니다.");
			break;
		}

		while (1) {
			f_key = _getch();
			if ((f_key) == '\r') break;
		}

		printf("\n");
		if (key == 'q') puts("qwer 화이팅");
		else if (key == 'w') puts("과제 너무 좋다");
		else if (key == 'e') puts("담주부턴 과제량 3배다");
		else if (key == 'r') puts("행복합니다");
		else puts("잘 못 입력하셨습니다.");
	}

}