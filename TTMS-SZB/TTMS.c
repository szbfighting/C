#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <io.h>
#include <conio.h>
#include "./View/Main_Menu.h"
#include "./View/Account_UI.h"


//定义全局变量，用于存储登陆用户信息
account_t gl_CurUser={0, USR_ANOMY, "Anonymous",""};

int main(void) {

	setvbuf(stdout, NULL, _IONBF, 0);
	system("color 27");
	printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
	printf("\t\t\t\t\t       欢迎使用嘿嘿嘿票务管理系统  \n\n");
	printf("\t\t----------------------------------------------------------------------------------------\n\n");
	printf("\n");
	printf("\n\n\n\t\t\t\t\t\t[N]\t匿名进入\n");
	printf("\n\n\n\t\t\t\t\t\t[S]\t实名进入\n");
	printf("\n\n\n\t\t****************************************************************************************\n");

	char ch;
	do
	{
		printf("\t\t你的选择：");
		ch=getchar();
		if (ch != 'n'&&ch != 'N'&&ch != 's'&&ch != 'S')
			printf("\t\t无效选项！\n\n");
		fflush(stdin);
		getchar();
	} while(ch!='n'&&ch!='N'&&ch!='s'&&ch!='S');
	

	if(ch=='S'||ch=='s')
	{
		if(!SysLogin()) {
			printf("\n\t\t对不起您无权登录本系统请按任意键退出......\n");
			getchar();
			exit(0);
		}
	}

	Main_Menu();
	return EXIT_SUCCESS;
}
