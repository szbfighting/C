#include "Play_UI.h"
#include "Schedule_UI.h"
#include "Sale_UI.h"
#include "Ticket_UI.h"
#include "../Service/ticket.h"
#include "../Service/seat.h"
#include "../Service/studio.h"
#include "../Service/EntityKey.h"
#include "../Service/Sale.h"
#include "../Common/common.h"
#include "../Common/list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//���õ�½�û���ȫ�����ݽṹ
extern account_t gl_CurUser;

//���ݼƻ�ID����ʾ�ݳ�Ʊ
void Sale_UI_ShowTicket(int schID)
{
	schedule_t* scheduleBuf = (schedule_t*)malloc(sizeof(schedule_t));
	Schedule_Srv_FetchByID(schID, scheduleBuf);

	seat_list_t seatList;
	List_Init(seatList, seat_node_t);
	Seat_Srv_FetchByRoomID(seatList, scheduleBuf->studio_id);

	ticket_node_t* pos;
	int i;
	char choice;
	Pagination_t paging;
	paging.offset = 0;
	paging.pageSize = TICKET_PAGE_SIZE;
	ticket_list_t ticketList;
	List_Init(ticketList, ticket_node_t);
	paging.totalRecords = Ticket_Srv_FetchBySchID(ticketList, schID);
	Paging_Locate_FirstPage(ticketList, paging);

	do {
		system("cls");

		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t\t\t\t\t      �ݳ�Ʊ�б� \n\n");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t%-30s  %-30s  %-30s\n", "�ݳ�ƱID", "Ʊ��", "�ݳ�Ʊ״̬");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		for (i = 0, pos = (ticket_node_t *) (paging.curPos);pos != ticketList && i < paging.pageSize; i++) {
			printf("\t\t%-10d  %-10d  ",pos->data.id, pos->data.price);
			switch(pos->data.status) {
			case TICKET_AVL:
				printf("%-10s","����");
				break;
			case TICKET_SOLD:
				printf("%-10s","����");
				break;
			case TICKET_RESV:
				printf("%-10s","Ԥ��");
				break;
			}
			printf("\n");
			pos = pos->next;
		}
		printf("\n\t\t========================�ܼ�¼:%2d ========================= ҳ %2d/%2d====================\n\n",
			paging.totalRecords, Pageing_CurPage(paging),
			Pageing_TotalPages(paging));
		printf("\tt\                  [B] �� Ʊ     |    [P] ��һҳ    |    [N] ��һҳ    |    [R] ����");
		printf("\t\t========================================================================================\n\n");
		printf("\t\t��ѡ��:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);
		switch (choice) {
		case 'b':
		case 'B':
			Sale_UI_SellTicket(ticketList, seatList);
			break;
		case 'p':
		case 'P':
			if (1 < Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(ticketList, paging, -1, ticket_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(ticketList, paging, 1, ticket_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
	List_Destroy(ticketList, ticket_node_t);
}

int Sale_UI_SellTicket(ticket_list_t tickList, seat_list_t seatList)
{

	int row, column;
	printf("\t\t��������λ�кţ�");
	scanf("%d",&row);
	printf("\t\t��������λ�кţ�");
	scanf("%d",&column);
	seat_node_t* findSeat = Seat_Srv_FindByRowCol(seatList, row, column);
	if( findSeat ==  NULL )
	{
		printf("\t\tδ�ҵ�����λ��\n");
		fflush(stdin);
		getchar();
		return 0;
	}

	ticket_node_t* findTicket = Ticket_Srv_FindBySeatID(tickList, findSeat->data.id);
	if(findTicket == NULL)
	{
		printf("\t\tδ�ҵ�Ʊ\n");
		fflush(stdin);
		getchar();
		return 0;
	}
	if(findTicket->data.status == 1)
	{
		printf("\t\t��Ʊ����\n");
		fflush(stdin);
		getchar();
		return 0;
	}

	fflush(stdin);
	printf("\t\tȷ�������Ʊ��\n");
	printf("\t\t��Y��ȷ��                                ��N������");
	char ch;
	do
	{
		ch = getchar();
	}while(ch!='Y'&&ch!='y'&&ch!='N'&&ch!='n');

	if(ch=='N'||ch=='n')
	{
		return 0;
	}

	findTicket->data.status = TICKET_SOLD;
	Ticket_Srv_Modify(&findTicket->data);

	sale_t newSale;

	newSale.id = EntKey_Srv_CompNewKey("Sale");
	newSale.ticket_id = findTicket->data.id;
	newSale.user_id = gl_CurUser.id;
	newSale.value = findTicket->data.price;
	newSale.type = SALE_SELL;
	newSale.date = DateNow();
	newSale.time = TimeNow();
	Sale_Srv_Add(&newSale);

	getchar();
	printf("\t\t��Ʊ�ɹ���\n");
	Ticket_UI_Print(findTicket->data.id);
	fflush(stdin);
	getchar();
	return 0;

}



//���ݾ�ĿID��ʾ�ݳ��ƻ�
void Sale_UI_ShowScheduler(int playID) {

	play_t buf;
	Play_Srv_FetchByID(playID, &buf);

	schedule_list_t head;
	List_Init(head, schedule_node_t);

	schedule_node_t* pos;
	int i;
	char choice;
	

	Pagination_t paging;
	paging.offset = 0;
	paging.pageSize = SCHEDULE_PAGE_SIZE;

	paging.totalRecords = Schedule_Srv_FetchByPlay(head, playID);

	Paging_Locate_FirstPage(head, paging);


	do {

		system("cls");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t\t\t\t\t      �ݳ��ƻ� \n\n");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t%-5s    %-5s        %-20s  %-13s  %-13s  %-5s\n", "�ƻ�ID", "��ӳ��ID", "��ĿID", "����", "ʱ��", "��λ����");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		for (i = 0, pos = (schedule_node_t *) (paging.curPos);pos != head && i < paging.pageSize; i++) {
			printf("\t\t%-5d  %-5d  %-30s  %-4d-%d-%-d    %-d:%d:%-d     %-5d\n",
					pos->data.id, pos->data.studio_id, buf.name, pos->data.date.year, pos->data.date.month, pos->data.date.day,
					pos->data.time.hour, pos->data.time.minute, pos->data.time.second, pos->data.seat_count);
					pos = pos->next;
		}
		printf("\n\t\t========================�ܼ�¼:%2d ========================= ҳ %2d/%2d====================\n\n",
			paging.totalRecords, Pageing_CurPage(paging),
			Pageing_TotalPages(paging));
		printf("\t\t                [t] �鿴�ݳ�Ʊ     |    [P] ��һҳ    |    [N] ��һҳ    |    [R] ����\n");
		printf("\t\t========================================================================================\n\n");
		printf("\t\tYour Choice:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		int schid;
		switch (choice)
		{
			case 't':
			case 'T':
				printf("\t\t�������ݳ��ƻ�ID��");
				scanf("%d",&schid);
				Sale_UI_ShowTicket(schid);
				break;
			case 'p':
			case 'P':
				if (1 < Pageing_CurPage(paging)) {
					Paging_Locate_OffsetPage(head, paging, -1, schedule_node_t);
				}
				break;
			case 'n':
			case 'N':
				if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
					Paging_Locate_OffsetPage(head, paging, 1, schedule_node_t);
				}
				break;
		}
	} while (choice != 'r' && choice != 'R');


	List_Destroy(head, schedule_node_t);

}

void Sale_UI_MgtEntry() {
	


//	char mf;
	char condt[100];
	char filter[100];
//	int index = 0;

	play_node_t* pos;
	Pagination_t paging;
	int i;
	int playID;
	char choice;
	play_t buf;
	play_list_t head;
	List_Init(head, play_node_t);



	paging.offset = 0;
	paging.pageSize = PLAY_PAGE_SIZE;

	paging.totalRecords = Play_Srv_FetchAll(head);

	Paging_Locate_FirstPage(head, paging);


		system("cls");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t\t\t\t\t      ��Ŀ�б� \n\n");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t%-5s  %-15s  %-10s  %-5s  %-5s  %-6s  %-6s  %-8s  %-12s\n\n", "ID", "��Ŀ��", "����",
			"ʱ��", "Ʊ��", "��Ŀ����", "��Ŀ����", "��ӳʱ��", "��ӳʱ��");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		for (i = 0, pos = (play_node_t *)(paging.curPos); pos != head && i < paging.pageSize; i++) {
			printf("\t\t%-5d  %-15s  %-10s  %-5d  %-5d",
				pos->data.id, pos->data.name, pos->data.area, pos->data.duration, pos->data.price);
			switch (pos->data.type) {
			case PLAY_TYPE_FILE:
				printf("  %-8s", "��Ӱ");
				break;
			case PLAY_TYPE_OPEAR:
				printf("  %-8s", "���");
				break;
			case PLAY_TYPE_CONCERT:
				printf("  %-8s", "���ֻ�");
				break;
			}

			switch (pos->data.rating) {
			case PLAY_RATE_CHILD:
				printf("  %-8s", "��ͯ");
				break;
			case PLAY_RATE_TEENAGE:
				printf("  %-8s", "������");
				break;
			case PLAY_RATE_ADULT:
				printf("  %-8s", "����");
				break;
			}
			printf("  %d��%d��%d��  %d��%d��%d��\n", pos->data.start_date.year, pos->data.start_date.month, pos->data.start_date.day,
				pos->data.end_date.year, pos->data.start_date.month, pos->data.start_date.day);
			pos = pos->next;
		}
		printf("\n\t\t========================�ܼ�¼:%2d ========================= ҳ %2d/%2d====================\n\n",
			paging.totalRecords, Pageing_CurPage(paging),
			Pageing_TotalPages(paging));
		printf("\t\t[P] ��һҳ  |  [N] ��һҳ  |  [C] �ݳ��ƻ�  |  [S] ��Ŀ����ѯ  |  [F] �� ��  |  [R] �� ��\n\n");
		printf("\t\t========================================================================================\n\n");
	do {
		printf("\t\t��ѡ��ѡ��:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice)
		{
		case 'c':
		case 'C':
			printf("\t\t��������鿴�ݳ��ƻ��ľ�ĿID��");
			scanf("%d",&playID);
			system("cls");
			Sale_UI_ShowScheduler(playID);
			break;
		case 's':
		case 'S':
			/*
			system("cls");
			while((mf=getch())!='\r')
			{
				condt[index]=mf;
				condt[index+1]='\0';
				index++;
				Play_Srv_FetchByName(head, condt);
				continue;
			}
			index = 0;*/
			printf("\t\t�������ѯ�ؼ��֣�");
			scanf("%s",condt);

			paging.totalRecords = Play_Srv_FetchByName(head, condt);
			List_Paging(head, paging, play_node_t);
			break;

		case 'f':
		case 'F':
			printf("\t\t��������˹ؼ��֣�");
			scanf("%s",filter);

			paging.totalRecords = Play_Srv_FilterByName(head, filter);
			List_Paging(head, paging, play_node_t);
			break;
		case 'p':
		case 'P':
			if (1 < Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, play_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, play_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');

	List_Destroy(head, play_node_t);


}

//��Ʊ
void Sale_UI_ReturnTicket()
{

	printf("\t\t������Ʊ��ID��");
	ticket_t *buf = (ticket_t*)malloc(sizeof(ticket_t));

	int id;
	scanf("%d",&id);
	fflush(stdin);
	if ( Ticket_Srv_FetchByID(id, buf) == 0)
	{
		printf("\t\tƱ�����ڣ�\n");
		getchar();
		return ;
	}

	if(buf->status == 0)
	{
		printf("\t\tƱ���ۣ�\n");
		getchar();
		return ;
	}
	if(buf->status == 9)
	{
		printf("\t\tƱԤ����\n");
		getchar();
		return ;
	}

	buf->status = TICKET_AVL;
	Ticket_Srv_Modify(buf);

	sale_t newSale;

	newSale.id = EntKey_Srv_CompNewKey("Sale");
	newSale.ticket_id = buf->id;
	newSale.user_id = gl_CurUser.id;
	newSale.value = buf->price;
	newSale.type = SALE_RETURN;
	newSale.date = DateNow();
	newSale.time = TimeNow();
	Sale_Srv_Add(&newSale);

	printf("\t\t��Ʊ�ɹ���\n");
	getchar();
	return ;

	
}
