#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <io.h>
#include <conio.h>
#include "./View/Main_Menu.h"
#include "./View/Account_UI.h"


//����ȫ�ֱ��������ڴ洢��½�û���Ϣ
account_t gl_CurUser={0, USR_ANOMY, "Anonymous",""};

int main(void) {

	setvbuf(stdout, NULL, _IONBF, 0);
	system("color 27");
	printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
	printf("\t\t\t\t\t       ��ӭʹ�úٺٺ�Ʊ�����ϵͳ  \n\n");
	printf("\t\t----------------------------------------------------------------------------------------\n\n");
	printf("\n");
	printf("\n\n\n\t\t\t\t\t\t[N]\t��������\n");
	printf("\n\n\n\t\t\t\t\t\t[S]\tʵ������\n");
	printf("\n\n\n\t\t****************************************************************************************\n");

	char ch;
	do
	{
		printf("\t\t���ѡ��");
		ch=getchar();
		if (ch != 'n'&&ch != 'N'&&ch != 's'&&ch != 'S')
			printf("\t\t��Чѡ�\n\n");
		fflush(stdin);
		getchar();
	} while(ch!='n'&&ch!='N'&&ch!='s'&&ch!='S');
	

	if(ch=='S'||ch=='s')
	{
		if(!SysLogin()) {
			printf("\n\t\t�Բ�������Ȩ��¼��ϵͳ�밴������˳�......\n");
			getchar();
			exit(0);
		}
	}

	Main_Menu();
	return EXIT_SUCCESS;
}
