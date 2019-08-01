
#include "SalesAnalysis_UI.h"
#include "../Common/list.h"
#include "../Common/common.h"
#include "../Service/SalesAnalysis.h"
#include <conio.h>
#include <stdio.h>
#include<io.h>
#include<process.h>
#include <assert.h>
#include <string.h>
#include <windows.h>

//�ⲿ����gl_CurUser ���ڴ洢��¼�û���Ϣ
extern account_t gl_CurUser;

//�����ҳ������ҳ���СΪ5
static const int SALESANALYSIS_PAGE_SIZE = 5;

//��Ժ�������а���,������ʾ��ֹĿǰ��Ժ��ӰƱ�����а�
void SalesAnalysis_UI_BoxOffice()
{
	salesanalysis_list_t head;
	char choice;
	salesanalysis_node_t* pos;
	int i;

	List_Init(head, salesanalysis_node_t);

	Pagination_t paging;
	paging.totalRecords = Salesanalysis_Srv_StaticSale(head);
	SalesAnalysis_Srv_SortBySale(head);

	paging.offset = 0;
	paging.pageSize = SALESANALYSIS_PAGE_SIZE;

	Paging_Locate_FirstPage(head, paging);

	do {

		system("cls");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t\t\t\t\t      Ʊ������ \n\n");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t%-5s  %-20s  %-10s  %-5s  %-5s  %-12s  %-12s\n", "ID", "��Ŀ��", "����",
			"��Ʊ��", "Ʊ��", "��ӳʱ��", "��ӳʱ��");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");

		for (i = 0, pos = (salesanalysis_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
			printf("\t\t%-5d  %-20s  %-10s  %-5ld  %-5ld  %5d��%d��%d�� %5d��%d��%d��\n",
					pos->data.play_id, pos->data.name, pos->data.area, pos->data.totaltickets, pos->data.sales,
					pos->data.start_date.year, pos->data.start_date.month, pos->data.start_date.day,
					pos->data.end_date.year, pos->data.start_date.month, pos->data.start_date.day);

			pos = pos->next;
		}
		printf("\n\t\t========================�ܼ�¼:%2d ========================= ҳ %2d/%2d====================\n\n",
			paging.totalRecords, Pageing_CurPage(paging),
			Pageing_TotalPages(paging));
		printf("\n\t\t\t\t   [N]  ��һҳ        [P]  ��һҳ        [R]  ���� \t\t\n");
		printf("\t\t========================================================================================\n\n");
		printf("\t\tYour Choice:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice)
		{

		case 'p':
		case 'P':
			if (1 < Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, salesanalysis_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, salesanalysis_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');

	List_Destroy(head, salesanalysis_node_t);

}

//��ʾ��ƱԱ�ڸ��������������Ʊ���
void SalesAnalysis_UI_StatSale(int usrID, user_date_t stDate, user_date_t endDate)
{

	account_t* usr=(account_t*)malloc(sizeof(account_t));
	int salecount;


	salecount = Salesanalysis_Srv_CompSaleVal(usrID, stDate, endDate);

	Account_Srv_FetchByID(usrID, usr);

	//getchar();
	printf("\t\t����Ա��%s\n",usr->username);
	printf("\t\t����ԱID��%d\n",usr->id);
	printf("\t\t�������۶%d\n\n",salecount);
	fflush(stdin);
	printf("\t\t�س�����...");
	getchar();
	return ;

}

//���۷�����ں�������ʾ�˵����˵�����"������ʾ��ֹĿǰ��Ժ��ӰƱ�����а�"������ʾ���ѯ������ƱԱ��Ʊ�������
//����ѯ������������ĳ��ƱԱ��Ʊ�����
void SalesAanalysis_UI_MgtEntry()
{


	char choice;
	char salerName[30];
	account_t buf;

	user_date_t nowTime = DateNow();
	user_date_t bTime;
	user_date_t eTime;

	do {
		system("cls");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t\t\t\t\t      ѡ���б� \n\n");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t\t\t\t\t[B]Ʊ������\n\t\t\t\t\t\t[D]�������۶�\n\t\t\t\t\t\t[S]�������۶�\n\t\t\t\t\t\t[R]����\n");
		printf("\t\t========================================================================================\n\n");
		printf("\t\tYour Choice:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'b':
		case 'B':
			if( gl_CurUser.type == USR_MANG )
			{
				SalesAnalysis_UI_BoxOffice();
			}
			else
			{
				getchar();
				printf("\t\t����Ȩʹ�ô˹��ܣ�\n�س�����\n");
				getchar();
			}
			break;
		case 'd':
		case 'D':
			if( gl_CurUser.type == USR_CLERK )
			{
				SalesAnalysis_UI_StatSale(gl_CurUser.id, nowTime, nowTime);

			}
			else
			{
				printf("\t\t��������Ҫ�鿴����ƱԱ�˻���\n");
				getchar();
				gets(salerName);

				if ( Account_Srv_FetchByName(salerName,&buf) == 1 )
				{

					SalesAnalysis_UI_StatSale(buf.id, nowTime, nowTime);
				}
				else
				{
					printf("\t\t�û������ڣ�\n�س�����\n");
					getchar();
				}

			}

			break;
		case 's':
		case 'S':
			//printf("%d",gl_CurUser.type);
			if( gl_CurUser.type ==  USR_MANG)
			{
				printf("\t\t��������Ҫ�鿴����ƱԱ�˺ţ�\n");
				getchar();
				gets(salerName);
				if ( Account_Srv_FetchByName(salerName,&buf) == 1)
				{
					printf("\t\t��������ʼʱ�䣺");
					scanf("%d %d %d",&bTime.year, &bTime.month, &bTime.day);
					printf("\t\t���������ʱ�䣺");
					scanf("%d %d %d",&eTime.year, &eTime.month, &eTime.day);

					fflush(stdin);
					SalesAnalysis_UI_StatSale(buf.id, bTime, eTime);

				}
				else
				{
					printf("\t\t�û������ڣ�\n�س�����\n");
					getchar();
				}
			}
			else
			{
				getchar();
				printf("\t\t����Ȩʹ�ô˹��ܣ�\n�س�����\n");
				getchar();
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');

	return;
}

