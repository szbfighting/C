

#include "Account_UI.h"
#include "../Common/list.h"
#include "../Service/Account.h"
#include "../Service/EntityKey.h"
#include "Account_UI.h"
#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include<io.h>
#include<process.h>
#include <time.h>
#include <conio.h>
#include <ctype.h>

//登录函数，提示用户输入用户名和密码，登录成功return 1，否则提示重新登录，超过3次，登录失败
int SysLogin() {

	Account_Srv_InitSys();

	int count;
	char ch;
	int i;
	char usrName[30];
	char pwd[30];
	for (count = 0; count < 3; count++) {
		system("cls");
		printf("\t\t\t╔ = = = = = = = = = = = = = = = = = = = = = = = = = ╗\n");
		printf("\t\t\t‖                    登    录                      ‖\n");
		printf("\t\t\t╚ = = = = = = = = = = = = = = = = = = = = = = = = = ╝\n");
		printf("\n\n");
		printf("\t\t\t\t○ 帐号：");

		i = 0;
		while (1) {
			ch = getch();
			if (ch == '\r' && i != 0) {
				usrName[i] = '\0';
				break;
			}

			if (ch == '\b') {
				if (i <= 0) {
					printf("\a");
					continue;
				}
				printf("\b \b");
				i--;
				continue;
			}
			if (i >= 30) {
				printf("\a");
				continue;
			}
			if (isprint(ch) && (isspace(ch))==0) {
				putchar(ch);
				usrName[i] = ch;
				i++;
			}
		}

		printf("\n\n\n\t\t\t\t○ 密码：  ");


		i = 0;
		while (1) {
			ch = getch();
			if (ch == '\r' && i != 0) {
				pwd[i] = '\0';
				break;
			}

			if (ch == '\b') {
				if (i == 0) {
					printf("\a");
					continue;
				}
				printf("\b \b");
				i--;
				continue;
			}
			if (i >= 30) {
				printf("\a");
				continue;
			}
			if ( isprint(ch) && (isspace(ch))==0 )
			{
				putchar('*');
				pwd[i] = ch;
				i++;
			}
		}
		printf("\n");

		if (Account_Srv_Verify(usrName, pwd) == 1) {

			return 1;
		}

	}

	return 0;

}

//管理系统用户功能入口函数，显示用户账号管理菜单
void Account_UI_MgtEntry() {


	account_list_t head;
	account_node_t* pos;
	Pagination_t paging;
	int i;

	char username[30];
	char choice;


	List_Init(head, account_node_t);
	paging.offset = 0;
	paging.pageSize = ACCOUNT_PAGE_SIZE;

	paging.totalRecords = Account_Srv_FetchAll(head);

	Paging_Locate_FirstPage(head, paging);

	do {

		system("cls");
		//printf(
			//	"\n=============================================================================================\n");
		printf(
				"\n                                 【    账    号    列    表    】                                    \n\n");
		printf(
				"============================================================================================\n");
		printf("%-5s  %-35s  %-35s  %-10s\n", "ID", "用户名", "密码",
				"用户类型");
		printf(
				"--------------------------------------------------------------------------------------------\n\n");

		for (i = 0, pos = (account_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
			printf("%-5d  %-35s  %-35s  ", pos->data.id, pos->data.username,
					pos->data.password);

			switch(pos->data.type)
			{
			case USR_ANOMY:
				printf("%-10s","USR_ANOMY");
				break;
			case USR_CLERK:
				printf("%-10s","USR_CLERK");
				break;
			case USR_MANG:
				printf("%-10s","USR_MANG");
				break;
			case USR_ADMIN:
				printf("%-10s","USR_ADMIN");
				break;
			}

			printf("\n");


			pos = pos->next;
		}

		printf(
				"\n--------------------- 总 记 录:%2d ----------------------- 页数 %2d/%2d -----------------------\n\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));

		printf(
				   "        [P]上一页   |   [N]下一页   |   [A]添加   |   [D]删除   |   [U]更新   |   [R]返回        ");
		printf(
				"\n=============================================================================================\n");
		printf("Your Choice:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice)
		{
		case 'a':
		case 'A':
			if (Account_UI_Add(head))
			{
				paging.totalRecords = Account_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, account_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("Input the user name:");
			scanf("%s", username);
			fflush(stdin);
			if (Account_UI_Delete(head, username)) {
				paging.totalRecords = Account_Srv_FetchAll(head);
				List_Paging(head, paging, account_node_t);
			}


			break;
		case 'u':
		case 'U':
			printf("输入账户名:");
			scanf("%s", username);
			if (Account_UI_Modify(head, username)) {	//从新载入数据
				paging.totalRecords = Account_Srv_FetchAll(head);
				List_Paging(head, paging, account_node_t);
			}

			break;
		case 'p':
		case 'P':
			if (1 < Pageing_CurPage(paging)) {
			//	paging.totalRecords = Account_Srv_FetchAll(head);
				Paging_Locate_OffsetPage(head, paging, -1, account_node_t);
			}

			break;
		case 'n':
		case 'N':
			if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
			//	paging.totalRecords = Account_Srv_FetchAll(head);
				Paging_Locate_OffsetPage(head, paging, 1, account_node_t);
			}

			break;
		}

		fflush(stdin);


	} while (choice != 'r' && choice != 'R');

	List_Destroy(head, account_node_t);

}

//添加一个用户账号信息，如果账号名存在，提示出错信息
int Account_UI_Add(account_list_t list) {

	account_t rec;
	int newRecCount = 0;
	char choice;

	int i;
	char ch;
	do {
		system("cls");
//		printf("\n\t\t=======================================================\n");
//		printf("\t\t****************       添 加 新 账 户         ****************\n");
//		printf("\t\t-------------------------------------------------------\n");
		printf("\n========================================================================================================================\n");
		printf("\n************************************************       添加新用户信息        ******************************************\n");
		printf("\n\t-----------------------------------------------------------------------------\n\n");
		printf("\t\t用户名:");
		fflush(stdin);
		//gets(rec.username);

		i = 0;
			while (1) {
				ch = getch();
				if (ch == '\r' && i != 0) {
					rec.username[i] = '\0';
					break;
				}

				if (ch == '\b') {
					if (i <= 0) {
						printf("\a");
						continue;
					}
					printf("\b \b");
					i--;
					continue;
				}
				if (i >= 29) {
					printf("\a");
					continue;
				}
				if (isprint(ch) && (isspace(ch))==0) {
					putchar(ch);
					rec.username[i] = ch;
					i++;
				}
			}

			printf("\n");


		if (Account_Srv_FindByUsrName(list, rec.username) != NULL) {
			printf("\t\t账号重复！\n");
			Sleep(500);
			return newRecCount;
		}

		printf("\t\t密码:");
		//gets(rec.password);

		i = 0;
			while (1) {
				ch = getch();
				if (ch == '\r' && i != 0) {
					rec.password[i] = '\0';
					break;
				}

				if (ch == '\b') {
					if (i == 0) {
						printf("\a");
						continue;
					}
					printf("\b \b");
					i--;
					continue;
				}
				if (i >= 29) {
					printf("\a");
					continue;
				}
				if ( isprint(ch) && (isspace(ch))==0 )
				{
					putchar('*');
					rec.password[i] = ch;
					i++;
				}
			}
			printf("\n");

	char typec;
		printf("\t\t用户类型(1:售票员 2:经理 9:系统管理员):");
		do
		{
			typec = getch();

		} while (typec!='0'&&typec!='1'&&typec!='2'&&typec!='9');
		putchar(typec);
		getchar();

		switch(typec)
		{
		case '0':
			rec.type = USR_ANOMY;
			break;
		case '1':
			rec.type = USR_CLERK;
			break;
		case '2':
			rec.type = USR_MANG;
			break;
		case '9':
			rec.type = USR_ADMIN;
			break;
		}

		printf("\n");




		printf("\t\t=======================================================\n");
		fflush(stdin);
		account_node_t* newnode = (account_node_t*)malloc(sizeof(account_node_t));
		newnode->data = rec;

		rec.id = EntKey_Srv_CompNewKey("Account");


		if (Account_Srv_Add(&rec)) {

			newRecCount += 1;
			List_AddTail(list,newnode);
//			getchar();
			printf("\t\t新用户添加成功!\n");
		} else
			printf("\t\t新用户添加失败!\n");
		printf("\t\t-------------------------------------------------------\n");
		printf("\t\t[A]再次添加, [R]返回:");
		fflush(stdin);
		scanf("%c", &choice);
	} while ('a' == choice || 'A' == choice);

	return newRecCount;

}

//根据用户账号名修改用户账号密码，不存在这个用户账号名，提示出错信息
int Account_UI_Modify(account_list_t list, char usrName[]) {

	account_t rec;
	int rtn = 0;
	char newname[30];
	char newpwd[30];


	/*Load record*/
	if (!Account_Srv_FetchByName(usrName, &rec)) {
		printf("\t\t账户不存在\n任意键返回!\n");
		getchar();
		return 0;
	}

//	printf("\n=======================================================\n");
//	printf("****************  更 新 账 户  ****************\n");
//	printf("-------------------------------------------------------\n");
	printf("\n========================================================================================================================\n");
	printf("\n************************************************       更新用户信息        ******************************************\n");
	printf("\n\t                    ---------------------------------------------------------------------------");

	printf("\t\t账号 ID[%d]\n", rec.id);


	int i;
	char ch;
	printf("\t\t用户名[%s]:", usrName);

	i = 0;
	while (1) {
		ch = getch();
		if (ch == '\r' && i != 0) {
			newname[i] = '\0';
			break;
		}

		if (ch == '\b') {
			if (i <= 0) {
				printf("\a");
				continue;
			}
			printf("\b \b");
			i--;
			continue;
		}
		if (i >= 30) {
			printf("\a");
			continue;
		}
		if (isprint(ch) && (isspace(ch))==0) {
			putchar(ch);
			newname[i] = ch;
			i++;
		}
	}

	printf("\n");
	fflush(stdin);
	if (Account_Srv_FindByUsrName(list, newname) != NULL) {
				printf("\t\t账户名重复！\n");
				Sleep(500);
				return 0;
			}

	strcpy(rec.username,newname);

	printf("用户密码[%s]:", rec.password);

	i = 0;
	while (1) {
		ch = getch();
		if (ch == '\r' && i != 0) {
			newpwd[i] = '\0';
			break;
		}

		if (ch == '\b') {
			if (i == 0) {
				printf("\a");
				continue;
			}
			printf("\b \b");
			i--;
			continue;
		}
		if (i >= 30) {
			printf("\a");
			continue;
		}
		if ( isprint(ch) && (isspace(ch))==0 )
		{
			putchar('*');
			newpwd[i] = ch;
			i++;
		}
	}
	printf("\n");
	strcpy(rec.password,newpwd);

	fflush(stdin);



	printf("\t\t用户类型[");

	switch(rec.type)
	{
	case USR_ANOMY:
		printf("%s","USR_ANOMY");
		break;
	case USR_CLERK:
		printf("%s","USR_CLERK");
		break;
	case USR_MANG:
		printf("%s","USR_MANG");
		break;
	case USR_ADMIN:
		printf("%s","USR_ADMIN");
		break;
	}

	printf("]\n");


	char typec;
	printf("User Type(0:匿名用户 1:售票员 2:经理 9:系统管理员):");


	do
	{
		typec = getch();

	} while (typec!='0'&&typec!='1'&&typec!='2'&&typec!='9');

	putchar(typec);

	getchar();

	switch(typec)
	{
	case '0':
		rec.type = USR_ANOMY;
		break;
	case '1':
		rec.type = USR_CLERK;
		break;
	case '2':
		rec.type = USR_MANG;
		break;
	case '9':
		rec.type = USR_ADMIN;
		break;
	}

	fflush(stdin);

	printf("\n");
	printf("\t\t-------------------------------------------------------\n");

	if (Account_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"\t\t用户信息更新成功!\n按[Enter]任意键继续!\n");
	}
	else
			printf("\t\t用户信息更新失败!\n按[Enter]任意键继续!\n");
	getchar();
	return rtn;


}

//根据用户账号名删除一个已经存在的用户账号信息，如果不存在这个用户账号名，提示出错信息
int Account_UI_Delete(account_list_t list, char usrName[]) {

	account_list_t acp;

	if ((acp = Account_Srv_FindByUsrName(list, usrName)) == NULL) {
		printf("\t\t账号不存在\n按任意键返回!\n");
		Sleep(500);
		return 0;
	}

//	List_DelNode(acp);

	if (Account_Srv_DeleteByID(acp->data.id) == 1)
	{
		return 1;
	}
	else
	{
		printf("\t\t删除失败！\n");
		Sleep(500);
		return 0;
	}

}

//根据用户账号名查找该用户账号是否存在，存在返回1，否则返回0，并提示错误信息
int Account_UI_Query(account_list_t list, char usrName[]) {

	if(Account_Srv_FindByUsrName(list,usrName)==NULL)return 0;
	else return 1;
}

