

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

//��¼��������ʾ�û������û��������룬��¼�ɹ�return 1��������ʾ���µ�¼������3�Σ���¼ʧ��
int SysLogin() {

	Account_Srv_InitSys();

	int count;
	char ch;
	int i;
	char usrName[30];
	char pwd[30];
	for (count = 0; count < 3; count++) {
		system("cls");
		printf("\t\t\t�X = = = = = = = = = = = = = = = = = = = = = = = = = �[\n");
		printf("\t\t\t��                    ��    ¼                      ��\n");
		printf("\t\t\t�^ = = = = = = = = = = = = = = = = = = = = = = = = = �a\n");
		printf("\n\n");
		printf("\t\t\t\t�� �ʺţ�");

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

		printf("\n\n\n\t\t\t\t�� ���룺  ");


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

//����ϵͳ�û�������ں�������ʾ�û��˺Ź���˵�
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
				"\n                                 ��    ��    ��    ��    ��    ��                                    \n\n");
		printf(
				"============================================================================================\n");
		printf("%-5s  %-35s  %-35s  %-10s\n", "ID", "�û���", "����",
				"�û�����");
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
				"\n--------------------- �� �� ¼:%2d ----------------------- ҳ�� %2d/%2d -----------------------\n\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));

		printf(
				   "        [P]��һҳ   |   [N]��һҳ   |   [A]���   |   [D]ɾ��   |   [U]����   |   [R]����        ");
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
			printf("�����˻���:");
			scanf("%s", username);
			if (Account_UI_Modify(head, username)) {	//������������
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

//���һ���û��˺���Ϣ������˺������ڣ���ʾ������Ϣ
int Account_UI_Add(account_list_t list) {

	account_t rec;
	int newRecCount = 0;
	char choice;

	int i;
	char ch;
	do {
		system("cls");
//		printf("\n\t\t=======================================================\n");
//		printf("\t\t****************       �� �� �� �� ��         ****************\n");
//		printf("\t\t-------------------------------------------------------\n");
		printf("\n========================================================================================================================\n");
		printf("\n************************************************       ������û���Ϣ        ******************************************\n");
		printf("\n\t-----------------------------------------------------------------------------\n\n");
		printf("\t\t�û���:");
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
			printf("\t\t�˺��ظ���\n");
			Sleep(500);
			return newRecCount;
		}

		printf("\t\t����:");
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
		printf("\t\t�û�����(1:��ƱԱ 2:���� 9:ϵͳ����Ա):");
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
			printf("\t\t���û���ӳɹ�!\n");
		} else
			printf("\t\t���û����ʧ��!\n");
		printf("\t\t-------------------------------------------------------\n");
		printf("\t\t[A]�ٴ����, [R]����:");
		fflush(stdin);
		scanf("%c", &choice);
	} while ('a' == choice || 'A' == choice);

	return newRecCount;

}

//�����û��˺����޸��û��˺����룬����������û��˺�������ʾ������Ϣ
int Account_UI_Modify(account_list_t list, char usrName[]) {

	account_t rec;
	int rtn = 0;
	char newname[30];
	char newpwd[30];


	/*Load record*/
	if (!Account_Srv_FetchByName(usrName, &rec)) {
		printf("\t\t�˻�������\n���������!\n");
		getchar();
		return 0;
	}

//	printf("\n=======================================================\n");
//	printf("****************  �� �� �� ��  ****************\n");
//	printf("-------------------------------------------------------\n");
	printf("\n========================================================================================================================\n");
	printf("\n************************************************       �����û���Ϣ        ******************************************\n");
	printf("\n\t                    ---------------------------------------------------------------------------");

	printf("\t\t�˺� ID[%d]\n", rec.id);


	int i;
	char ch;
	printf("\t\t�û���[%s]:", usrName);

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
				printf("\t\t�˻����ظ���\n");
				Sleep(500);
				return 0;
			}

	strcpy(rec.username,newname);

	printf("�û�����[%s]:", rec.password);

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



	printf("\t\t�û�����[");

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
	printf("User Type(0:�����û� 1:��ƱԱ 2:���� 9:ϵͳ����Ա):");


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
				"\t\t�û���Ϣ���³ɹ�!\n��[Enter]���������!\n");
	}
	else
			printf("\t\t�û���Ϣ����ʧ��!\n��[Enter]���������!\n");
	getchar();
	return rtn;


}

//�����û��˺���ɾ��һ���Ѿ����ڵ��û��˺���Ϣ���������������û��˺�������ʾ������Ϣ
int Account_UI_Delete(account_list_t list, char usrName[]) {

	account_list_t acp;

	if ((acp = Account_Srv_FindByUsrName(list, usrName)) == NULL) {
		printf("\t\t�˺Ų�����\n�����������!\n");
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
		printf("\t\tɾ��ʧ�ܣ�\n");
		Sleep(500);
		return 0;
	}

}

//�����û��˺������Ҹ��û��˺��Ƿ���ڣ����ڷ���1�����򷵻�0������ʾ������Ϣ
int Account_UI_Query(account_list_t list, char usrName[]) {

	if(Account_Srv_FindByUsrName(list,usrName)==NULL)return 0;
	else return 1;
}

