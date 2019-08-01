#include "Ticket_UI.h"

#include "../Common/list.h"
#include "../Service/ticket.h"
#include "../Service/schedule.h"
#include "../Service/play.h"
#include "../Service/studio.h"

#include <stdio.h>
#include <stdlib.h>

//根据状态返回对应表示状态符号
char Ticket_UI_Status2Char(ticket_status_t status) {

	char statusChar;
	switch (status) {
	case TICKET_AVL:		//有座位
		statusChar = 'O';
		break;
	case TICKET_SOLD:	//损坏的座位
		statusChar = 'X';
		break;
	case TICKET_RESV:
	default:
		statusChar = ' ';
		break;
	}
	return statusChar;
}

void Ticket_UI_Print(int ID){
	//根据ID载入票
	ticket_t ticket;
	if(!Ticket_Srv_FetchByID(ID, &ticket)){
		printf("\t\t该票不存在！\n\t\t按[Enter]键返回\n ");
		return ;
	}

	//载入演出技计划及剧目
	schedule_t sch;
	play_t play;
	Schedule_Srv_FetchByID(ticket.schedule_id, &sch);
	Play_Srv_FetchByID(sch.play_id, &play);

	//载入座位及演出厅
	seat_t seat;
	studio_t room;
	Seat_Srv_FetchByID(ticket.seat_id, &seat);
	Studio_Srv_FetchByID(seat.roomID, &room);

	//显示票信息
	printf("\n\t\t|----------------------------------------------------------------------------------------|");
	printf("\n\t\t|\t\tID:%-9d %10s：%d %10s：%d                             |"
		, ticket.id, "行数:", seat.row, "列数:", seat.column );
	printf("\n\t\t|\t\t%-9s%-39s                         |", "票名:", play.name);
	printf("\n\t\t|\t\t%-9s%-11d %10d年%2d月%2d  %2d:%2d                           |", "票价:", play.price,
				sch.date.year, sch.date.month, sch.date.day,
				sch.time.hour, sch.time.minute);
	printf("\n\t\t|----------------------------------------------------------------------------------------|\n\n");
}

//根据计划ID显示所有票
void Ticket_UI_MgtEntry(int schedule_id) {
	system("cls");
	schedule_t buf;
	play_t play;
	printf("\n\t\t                                剧    目    信    息    \n\n");
	printf("\t\t========================================================================================\n\n");
	if (Schedule_Srv_FetchByID(schedule_id, &buf)) {
		if (Play_Srv_FetchByID(buf.play_id, &play)) {
			printf("\t\t%-30s  %-20s  %-20s  %-20s", "剧目名称", "演出厅编号", "演出日期", "演出时间");
			printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
			printf("\t\t%-30s  %-20d  %-4d-%02d-%02d\t    %-02d:%-02d:%-02d\n", play.name, buf.id
				, buf.date.year, buf.date.month, buf.date.day
				, buf.time.hour, buf.time.minute, buf.time.second);
			printf("\t\t========================================================================================\n\n");
			printf("\t\t        [G] 生成演出票                                            [R]重新生成演出票\n");
			printf("\t\t========================================================================================\n\n");
			char choice;
			printf("\t\t你的选择：");
			fflush(stdin);
			//getchar();
			choice = getchar();
			switch (choice) {
			case 'g':
			case 'G':
				if (!Ticket_Srv_GenBatch(schedule_id)) {
					printf("\t\t生成演出票失败!\n");
				}
				break;
			case 'r':
			case 'R':
				Ticket_Srv_DeleteBatch(schedule_id);
				if (!Ticket_Srv_GenBatch(schedule_id)) {
					printf("\t\t重新生成演出票失败!\n");
				}
				break;
			default:
				printf("\t\t输入错误\n");
				break;
			}
		} else {
			printf("\t\t查询剧目失败！\n");
		}
	} else {
		printf("\t\t查询演出失败！\n");
	}
	printf("\t\t");
	system("pause");
}

void Ticket_UI_ListBySch(const schedule_t *sch,	ticket_list_t tickList, seat_list_t seatList) {
	assert(NULL!=sch && NULL!=tickList && NULL!=seatList);
	int i, j;

	studio_t studioRec;
	play_t   playRec;
	ticket_node_t *pTicket;
	seat_node_t *pSeat;

	if (!Studio_Srv_FetchByID(sch->studio_id, &studioRec)) {  //获得对应id放映厅的信息
		printf("\t\t该演出厅不存在!\n\t\t按[Enter]键继续!\n");
		getchar();
		return;
	}

	if (!Play_Srv_FetchByID(sch->play_id, &playRec)) {  //获得对应id剧目的信息
		printf("\t\t该剧目不存在!\n\t\t按[Enter]键继续!\n");
		getchar();
		return;
	}

	printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
	printf("\t\t\t\t\t         Ticket List for %s \n\n", playRec.name);
	printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
	printf("%5c", ' ');
	for (i = 1; i <= studioRec.colsCount; i++) {
		printf("%3d", i);
	}

	printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
	//显示数据
	for (i = 1; i <= studioRec.rowsCount; i++) {
		j = 1;
		printf("%2d:%c", i, '|');
		List_ForEach(tickList, pTicket)
		{
			pSeat=Seat_Srv_FindByID(seatList, pTicket->data.seat_id);
			if(NULL!=pSeat && pSeat->data.row == i) {
				while (pSeat->data.column != j) {
					printf("%3c", ' ');
					j++;
				}
				printf("%3c", Ticket_UI_Status2Char(pTicket->data.status));
				j++;
			}
		}
		printf("\n");
	}
	printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
}

void ListTickets(void){
	int i, schedule_id;
	schedule_t schedule_rec;
	play_t play_rec;
	char choice;

	ticket_list_t head;
	ticket_node_t *pos;
	Pagination_t paging;

	List_Init(head, ticket_node_t);
	paging.offset = 0;
	paging.pageSize = TICKET_PAGE_SIZE;

	printf("\t\t请输入演出计划ID:");
	scanf("%d",&schedule_id);

	//载入数据
	paging.totalRecords = Ticket_Srv_FetchBySchID(head,schedule_id);
	Paging_Locate_FirstPage(head, paging);
	//需要增加查找座位信息

	Schedule_Srv_FetchByID(schedule_id,&schedule_rec);
	Play_Srv_FetchByID(schedule_rec.play_id,&play_rec);
	do {
		system("cls");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\n\t\t\t\t\t           票务信息列表\n");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\n");
		printf("\t\t%-10s%-10s%14s%16s%21s%11s\n", "ID", "剧目名","日期", "时间", "售价", "状态");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		//显示数据
		for (i = 0, pos = (ticket_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
			printf("\t\t%-10d%-10s%10d年%02d月%02d%10d:%-10d%10d%15s\n", pos->data.id, play_rec.name,
					schedule_rec.date.year, schedule_rec.date.month, schedule_rec.date.day
				, schedule_rec.time.hour, schedule_rec.time.minute
				, pos->data.price, pos->data.status==0?"to sell":"sold");
			pos = pos->next;
		}
		printf("\n\t\t========================总记录:%2d ========================= 页 %2d/%2d====================\n\n",
				paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
		//printf("[P]revPage|[N]extPage | [U]pdate | [R]eturn ");
		printf("\n\t\t\t\t   [N]  下一页        [P]  上一页        [R]  返回 \t\t\n");
		fflush(stdin);
		getchar();
		printf("\t\t你的选择：");
		scanf("%c", &choice);
		switch (choice) {
	/*	case 'u':
		case 'U':
			printf("\t\t请输入票的ID:");
			scanf("%d", &id);
			if (UpdateTicket(id)) {	//从新载入数据
				paging.totalRecords = Ticket_Srv_FetchBySchID(head,schedule_rec.id);
				List_Paging(head, paging, ticket_node_t);
			}
			break;
	*/	case 'p':
		case 'P':
			if (1 < Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, ticket_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, ticket_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
	//释放链表空间
	List_Destroy(head, ticket_node_t);
	}


int UpdateTicket(int id){
	ticket_t rec;
	schedule_t schedule_rec;
	play_t play_rec;
	int rtn = 0;


	/*Load record*/
	if (!Ticket_Srv_FetchByID(id, &rec)) {
		printf("该剧目不存在!\n按 [Enter]键继续!\n");
		getchar();
		return 0;
	}
	Schedule_Srv_FetchByID(rec.schedule_id,&schedule_rec);
	Play_Srv_FetchByID(schedule_rec.play_id,&play_rec);
	//需要增加查找座位信息

	printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
	printf("\n\t\t\t\t\t\t    更新票务信息\n");
	printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
	printf("\t\tID:%d\n", rec.id);
	printf("\t\t剧目名称[%s]\n", play_rec.name);
	//需要输出座位的行列号
	printf("\t\t演出日期[%4d-%02d-%02d]\n",schedule_rec.date.year,
			schedule_rec.date.month,schedule_rec.date.day);
	printf("\t\t演出时间[%02d:%02d]\n",schedule_rec.time.hour,schedule_rec.time.minute);
	printf("\t\t票价[%-2d]:",rec.price);
	scanf("%d",&(rec.price));
	printf("\t\t状态[%s]:",rec.status==1?"to sell":"sold");
	scanf("%d",&rec.status);
	printf("\n\t\t----------------------------------------------------------------------------------------\n\n");

	if (Ticket_Srv_Modify(&rec)) {
		rtn = 1;
		printf("\t\t票务信息更新成功啦!\n\t\t按[Enter]键返回!\n");
	} else
		printf("\t\t票务信息更新失败!\n\t\t按[Enter]键返回!\n");
	getchar();
	return rtn;
}


int QueryTicket(int id){

	int rtn=0;
	schedule_t schedule_rec;
	play_t play_rec;
	ticket_t rec;
	if (Ticket_Srv_FetchByID(id,&rec)) {
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\n\t\t\t\t\t\t     票务信息\n");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t%-5s%20s%20s%20s%10s%10s\n", "ID", "剧目名称", "日期", "时间", "票价", "状态");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");

		Schedule_Srv_FetchByID(id,&schedule_rec);
		Play_Srv_FetchByID(schedule_rec.play_id,&play_rec);
		//添加查找座位行列信息的函数
		printf("\t\t%-5d%17s%20d年%02d月%02d日%10d:%02d%8d%15s\n", rec.id, play_rec.name
			, schedule_rec.date.year, schedule_rec.date.month, schedule_rec.date.day
			, schedule_rec.time.hour, schedule_rec.time.minute
			, rec.price, rec.status);
		rtn = 1;
	}
	else
	{
		printf("\t\t不存在这张票!\n\t\t按[Enter]键继续!\n");
	}
	printf("\n\t\t----------------------------------------------------------------------------------------\n\n");

	system("pause");
	return rtn;
}
