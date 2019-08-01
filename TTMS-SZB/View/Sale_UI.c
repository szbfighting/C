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

//引用登陆用户的全局数据结构
extern account_t gl_CurUser;

//根据计划ID，显示演出票
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
		printf("\t\t\t\t\t\t      演出票列表 \n\n");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t%-30s  %-30s  %-30s\n", "演出票ID", "票价", "演出票状态");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		for (i = 0, pos = (ticket_node_t *) (paging.curPos);pos != ticketList && i < paging.pageSize; i++) {
			printf("\t\t%-10d  %-10d  ",pos->data.id, pos->data.price);
			switch(pos->data.status) {
			case TICKET_AVL:
				printf("%-10s","待售");
				break;
			case TICKET_SOLD:
				printf("%-10s","已售");
				break;
			case TICKET_RESV:
				printf("%-10s","预留");
				break;
			}
			printf("\n");
			pos = pos->next;
		}
		printf("\n\t\t========================总记录:%2d ========================= 页 %2d/%2d====================\n\n",
			paging.totalRecords, Pageing_CurPage(paging),
			Pageing_TotalPages(paging));
		printf("\tt\                  [B] 购 票     |    [P] 上一页    |    [N] 下一页    |    [R] 返回");
		printf("\t\t========================================================================================\n\n");
		printf("\t\t请选择:");
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
	printf("\t\t请输入座位行号：");
	scanf("%d",&row);
	printf("\t\t请输入座位列号：");
	scanf("%d",&column);
	seat_node_t* findSeat = Seat_Srv_FindByRowCol(seatList, row, column);
	if( findSeat ==  NULL )
	{
		printf("\t\t未找到此座位！\n");
		fflush(stdin);
		getchar();
		return 0;
	}

	ticket_node_t* findTicket = Ticket_Srv_FindBySeatID(tickList, findSeat->data.id);
	if(findTicket == NULL)
	{
		printf("\t\t未找到票\n");
		fflush(stdin);
		getchar();
		return 0;
	}
	if(findTicket->data.status == 1)
	{
		printf("\t\t此票已售\n");
		fflush(stdin);
		getchar();
		return 0;
	}

	fflush(stdin);
	printf("\t\t确定购买此票？\n");
	printf("\t\t【Y】确定                                【N】返回");
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
	printf("\t\t购票成功！\n");
	Ticket_UI_Print(findTicket->data.id);
	fflush(stdin);
	getchar();
	return 0;

}



//根据剧目ID显示演出计划
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
		printf("\t\t\t\t\t\t      演出计划 \n\n");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t%-5s    %-5s        %-20s  %-13s  %-13s  %-5s\n", "计划ID", "放映厅ID", "剧目ID", "日期", "时间", "座位数量");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		for (i = 0, pos = (schedule_node_t *) (paging.curPos);pos != head && i < paging.pageSize; i++) {
			printf("\t\t%-5d  %-5d  %-30s  %-4d-%d-%-d    %-d:%d:%-d     %-5d\n",
					pos->data.id, pos->data.studio_id, buf.name, pos->data.date.year, pos->data.date.month, pos->data.date.day,
					pos->data.time.hour, pos->data.time.minute, pos->data.time.second, pos->data.seat_count);
					pos = pos->next;
		}
		printf("\n\t\t========================总记录:%2d ========================= 页 %2d/%2d====================\n\n",
			paging.totalRecords, Pageing_CurPage(paging),
			Pageing_TotalPages(paging));
		printf("\t\t                [t] 查看演出票     |    [P] 上一页    |    [N] 下一页    |    [R] 返回\n");
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
				printf("\t\t请输入演出计划ID：");
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
		printf("\t\t\t\t\t\t      剧目列表 \n\n");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t%-5s  %-15s  %-10s  %-5s  %-5s  %-6s  %-6s  %-8s  %-12s\n\n", "ID", "剧目名", "区域",
			"时长", "票价", "剧目类型", "剧目级别", "上映时间", "下映时间");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		for (i = 0, pos = (play_node_t *)(paging.curPos); pos != head && i < paging.pageSize; i++) {
			printf("\t\t%-5d  %-15s  %-10s  %-5d  %-5d",
				pos->data.id, pos->data.name, pos->data.area, pos->data.duration, pos->data.price);
			switch (pos->data.type) {
			case PLAY_TYPE_FILE:
				printf("  %-8s", "电影");
				break;
			case PLAY_TYPE_OPEAR:
				printf("  %-8s", "歌剧");
				break;
			case PLAY_TYPE_CONCERT:
				printf("  %-8s", "音乐会");
				break;
			}

			switch (pos->data.rating) {
			case PLAY_RATE_CHILD:
				printf("  %-8s", "儿童");
				break;
			case PLAY_RATE_TEENAGE:
				printf("  %-8s", "青少年");
				break;
			case PLAY_RATE_ADULT:
				printf("  %-8s", "成人");
				break;
			}
			printf("  %d年%d月%d日  %d年%d月%d日\n", pos->data.start_date.year, pos->data.start_date.month, pos->data.start_date.day,
				pos->data.end_date.year, pos->data.start_date.month, pos->data.start_date.day);
			pos = pos->next;
		}
		printf("\n\t\t========================总记录:%2d ========================= 页 %2d/%2d====================\n\n",
			paging.totalRecords, Pageing_CurPage(paging),
			Pageing_TotalPages(paging));
		printf("\t\t[P] 上一页  |  [N] 下一页  |  [C] 演出计划  |  [S] 剧目名查询  |  [F] 过 滤  |  [R] 返 回\n\n");
		printf("\t\t========================================================================================\n\n");
	do {
		printf("\t\t请选择选项:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice)
		{
		case 'c':
		case 'C':
			printf("\t\t请输入需查看演出计划的剧目ID：");
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
			printf("\t\t请输入查询关键字：");
			scanf("%s",condt);

			paging.totalRecords = Play_Srv_FetchByName(head, condt);
			List_Paging(head, paging, play_node_t);
			break;

		case 'f':
		case 'F':
			printf("\t\t请输入过滤关键字：");
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

//退票
void Sale_UI_ReturnTicket()
{

	printf("\t\t请输入票的ID：");
	ticket_t *buf = (ticket_t*)malloc(sizeof(ticket_t));

	int id;
	scanf("%d",&id);
	fflush(stdin);
	if ( Ticket_Srv_FetchByID(id, buf) == 0)
	{
		printf("\t\t票不存在！\n");
		getchar();
		return ;
	}

	if(buf->status == 0)
	{
		printf("\t\t票待售！\n");
		getchar();
		return ;
	}
	if(buf->status == 9)
	{
		printf("\t\t票预留！\n");
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

	printf("\t\t退票成功！\n");
	getchar();
	return ;

	
}
