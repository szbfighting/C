#include "Ticket_UI.h"

#include "../Common/list.h"
#include "../Service/ticket.h"
#include "../Service/schedule.h"
#include "../Service/play.h"
#include "../Service/studio.h"

#include <stdio.h>
#include <stdlib.h>

//����״̬���ض�Ӧ��ʾ״̬����
char Ticket_UI_Status2Char(ticket_status_t status) {

	char statusChar;
	switch (status) {
	case TICKET_AVL:		//����λ
		statusChar = 'O';
		break;
	case TICKET_SOLD:	//�𻵵���λ
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
	//����ID����Ʊ
	ticket_t ticket;
	if(!Ticket_Srv_FetchByID(ID, &ticket)){
		printf("\t\t��Ʊ�����ڣ�\n\t\t��[Enter]������\n ");
		return ;
	}

	//�����ݳ����ƻ�����Ŀ
	schedule_t sch;
	play_t play;
	Schedule_Srv_FetchByID(ticket.schedule_id, &sch);
	Play_Srv_FetchByID(sch.play_id, &play);

	//������λ���ݳ���
	seat_t seat;
	studio_t room;
	Seat_Srv_FetchByID(ticket.seat_id, &seat);
	Studio_Srv_FetchByID(seat.roomID, &room);

	//��ʾƱ��Ϣ
	printf("\n\t\t|----------------------------------------------------------------------------------------|");
	printf("\n\t\t|\t\tID:%-9d %10s��%d %10s��%d                             |"
		, ticket.id, "����:", seat.row, "����:", seat.column );
	printf("\n\t\t|\t\t%-9s%-39s                         |", "Ʊ��:", play.name);
	printf("\n\t\t|\t\t%-9s%-11d %10d��%2d��%2d  %2d:%2d                           |", "Ʊ��:", play.price,
				sch.date.year, sch.date.month, sch.date.day,
				sch.time.hour, sch.time.minute);
	printf("\n\t\t|----------------------------------------------------------------------------------------|\n\n");
}

//���ݼƻ�ID��ʾ����Ʊ
void Ticket_UI_MgtEntry(int schedule_id) {
	system("cls");
	schedule_t buf;
	play_t play;
	printf("\n\t\t                                ��    Ŀ    ��    Ϣ    \n\n");
	printf("\t\t========================================================================================\n\n");
	if (Schedule_Srv_FetchByID(schedule_id, &buf)) {
		if (Play_Srv_FetchByID(buf.play_id, &play)) {
			printf("\t\t%-30s  %-20s  %-20s  %-20s", "��Ŀ����", "�ݳ������", "�ݳ�����", "�ݳ�ʱ��");
			printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
			printf("\t\t%-30s  %-20d  %-4d-%02d-%02d\t    %-02d:%-02d:%-02d\n", play.name, buf.id
				, buf.date.year, buf.date.month, buf.date.day
				, buf.time.hour, buf.time.minute, buf.time.second);
			printf("\t\t========================================================================================\n\n");
			printf("\t\t        [G] �����ݳ�Ʊ                                            [R]���������ݳ�Ʊ\n");
			printf("\t\t========================================================================================\n\n");
			char choice;
			printf("\t\t���ѡ��");
			fflush(stdin);
			//getchar();
			choice = getchar();
			switch (choice) {
			case 'g':
			case 'G':
				if (!Ticket_Srv_GenBatch(schedule_id)) {
					printf("\t\t�����ݳ�Ʊʧ��!\n");
				}
				break;
			case 'r':
			case 'R':
				Ticket_Srv_DeleteBatch(schedule_id);
				if (!Ticket_Srv_GenBatch(schedule_id)) {
					printf("\t\t���������ݳ�Ʊʧ��!\n");
				}
				break;
			default:
				printf("\t\t�������\n");
				break;
			}
		} else {
			printf("\t\t��ѯ��Ŀʧ�ܣ�\n");
		}
	} else {
		printf("\t\t��ѯ�ݳ�ʧ�ܣ�\n");
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

	if (!Studio_Srv_FetchByID(sch->studio_id, &studioRec)) {  //��ö�Ӧid��ӳ������Ϣ
		printf("\t\t���ݳ���������!\n\t\t��[Enter]������!\n");
		getchar();
		return;
	}

	if (!Play_Srv_FetchByID(sch->play_id, &playRec)) {  //��ö�Ӧid��Ŀ����Ϣ
		printf("\t\t�þ�Ŀ������!\n\t\t��[Enter]������!\n");
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
	//��ʾ����
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

	printf("\t\t�������ݳ��ƻ�ID:");
	scanf("%d",&schedule_id);

	//��������
	paging.totalRecords = Ticket_Srv_FetchBySchID(head,schedule_id);
	Paging_Locate_FirstPage(head, paging);
	//��Ҫ���Ӳ�����λ��Ϣ

	Schedule_Srv_FetchByID(schedule_id,&schedule_rec);
	Play_Srv_FetchByID(schedule_rec.play_id,&play_rec);
	do {
		system("cls");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\n\t\t\t\t\t           Ʊ����Ϣ�б�\n");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\n");
		printf("\t\t%-10s%-10s%14s%16s%21s%11s\n", "ID", "��Ŀ��","����", "ʱ��", "�ۼ�", "״̬");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		//��ʾ����
		for (i = 0, pos = (ticket_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
			printf("\t\t%-10d%-10s%10d��%02d��%02d%10d:%-10d%10d%15s\n", pos->data.id, play_rec.name,
					schedule_rec.date.year, schedule_rec.date.month, schedule_rec.date.day
				, schedule_rec.time.hour, schedule_rec.time.minute
				, pos->data.price, pos->data.status==0?"to sell":"sold");
			pos = pos->next;
		}
		printf("\n\t\t========================�ܼ�¼:%2d ========================= ҳ %2d/%2d====================\n\n",
				paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
		//printf("[P]revPage|[N]extPage | [U]pdate | [R]eturn ");
		printf("\n\t\t\t\t   [N]  ��һҳ        [P]  ��һҳ        [R]  ���� \t\t\n");
		fflush(stdin);
		getchar();
		printf("\t\t���ѡ��");
		scanf("%c", &choice);
		switch (choice) {
	/*	case 'u':
		case 'U':
			printf("\t\t������Ʊ��ID:");
			scanf("%d", &id);
			if (UpdateTicket(id)) {	//������������
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
	//�ͷ�����ռ�
	List_Destroy(head, ticket_node_t);
	}


int UpdateTicket(int id){
	ticket_t rec;
	schedule_t schedule_rec;
	play_t play_rec;
	int rtn = 0;


	/*Load record*/
	if (!Ticket_Srv_FetchByID(id, &rec)) {
		printf("�þ�Ŀ������!\n�� [Enter]������!\n");
		getchar();
		return 0;
	}
	Schedule_Srv_FetchByID(rec.schedule_id,&schedule_rec);
	Play_Srv_FetchByID(schedule_rec.play_id,&play_rec);
	//��Ҫ���Ӳ�����λ��Ϣ

	printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
	printf("\n\t\t\t\t\t\t    ����Ʊ����Ϣ\n");
	printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
	printf("\t\tID:%d\n", rec.id);
	printf("\t\t��Ŀ����[%s]\n", play_rec.name);
	//��Ҫ�����λ�����к�
	printf("\t\t�ݳ�����[%4d-%02d-%02d]\n",schedule_rec.date.year,
			schedule_rec.date.month,schedule_rec.date.day);
	printf("\t\t�ݳ�ʱ��[%02d:%02d]\n",schedule_rec.time.hour,schedule_rec.time.minute);
	printf("\t\tƱ��[%-2d]:",rec.price);
	scanf("%d",&(rec.price));
	printf("\t\t״̬[%s]:",rec.status==1?"to sell":"sold");
	scanf("%d",&rec.status);
	printf("\n\t\t----------------------------------------------------------------------------------------\n\n");

	if (Ticket_Srv_Modify(&rec)) {
		rtn = 1;
		printf("\t\tƱ����Ϣ���³ɹ���!\n\t\t��[Enter]������!\n");
	} else
		printf("\t\tƱ����Ϣ����ʧ��!\n\t\t��[Enter]������!\n");
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
		printf("\n\t\t\t\t\t\t     Ʊ����Ϣ\n");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t%-5s%20s%20s%20s%10s%10s\n", "ID", "��Ŀ����", "����", "ʱ��", "Ʊ��", "״̬");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");

		Schedule_Srv_FetchByID(id,&schedule_rec);
		Play_Srv_FetchByID(schedule_rec.play_id,&play_rec);
		//��Ӳ�����λ������Ϣ�ĺ���
		printf("\t\t%-5d%17s%20d��%02d��%02d��%10d:%02d%8d%15s\n", rec.id, play_rec.name
			, schedule_rec.date.year, schedule_rec.date.month, schedule_rec.date.day
			, schedule_rec.time.hour, schedule_rec.time.minute
			, rec.price, rec.status);
		rtn = 1;
	}
	else
	{
		printf("\t\t����������Ʊ!\n\t\t��[Enter]������!\n");
	}
	printf("\n\t\t----------------------------------------------------------------------------------------\n\n");

	system("pause");
	return rtn;
}
