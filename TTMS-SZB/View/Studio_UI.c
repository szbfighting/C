
#include "Studio_UI.h"

#include "../Common/list.h"
#include "../Service/studio.h"
#include "../Service/seat.h"
#include "../Service/EntityKey.h"
#include "Seat_UI.h"

static const int STUDIO_PAGE_SIZE = 5;

#include <stdio.h>

void Studio_UI_MgtEntry(void) {




	int i, id;
	char choice;

	studio_list_t head;
	studio_node_t *pos;
	Pagination_t paging;

	List_Init(head, studio_node_t);
	paging.offset = 0;
	paging.pageSize = STUDIO_PAGE_SIZE;

	//��������
	paging.totalRecords = Studio_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do {
		system("cls");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t\t\t\t\t      �ݳ����б� \n\n");
		printf("\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t   %-10s  %-25s  %-15s  %-15s  %-15s\n\n", "ID", "�ݳ�����", "����", "����", "��λ����");
		printf("\t\t========================================================================================\n\n");
		//��ʾ����
		for (i = 0, pos = (studio_node_t *) (paging.curPos); pos != head && i < paging.pageSize; i++) {
			printf("\t\t   %-10d  %-25s  %-15d  %-15d  %-15d\n"
				, pos->data.id, pos->data.name, pos->data.rowsCount, pos->data.colsCount, pos->data.seatsCount);
			pos = pos->next;
		}
		printf("\n\t\t========================�ܼ�¼:%2d ========================= ҳ %2d/%2d====================\n\n"
			, paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
		printf("\n\t\t\t\t[A]\t�����µ��ݳ���\t\t[D]\tɾ���ݳ���\n\n"
				"\t\t\t\t[U]\t����\t\t\t[S]\t��λ\n\n"
				"\t\t\t\t[N]\t��һҳ\t\t\t[P]\t��һҳ\n\n"
				"\t\t\t\t[R]\t���� \t\t\n");
		printf("\n\t\t���ѡ��:");

		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);
		
		switch (choice) {
		case 'a':
		case 'A':
			if (Studio_UI_Add()) //����ӳɹ����������һҳ��ʾ
			{
				paging.totalRecords = Studio_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, studio_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("\n\t\t�������ݳ�����ID:");
			scanf("%d", &id);
			if (Studio_UI_Delete(id)) {	//������������
				paging.totalRecords = Studio_Srv_FetchAll(head);
				List_Paging(head, paging, studio_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("\n\t\t�������ݳ�����ID:");
			scanf("%d", &id);
			if (Studio_UI_Modify(id)) {	//������������
				paging.totalRecords = Studio_Srv_FetchAll(head);
				List_Paging(head, paging, studio_node_t);
			}
			break;
		case 's':
		case 'S':
			printf("\n\t\t�������ݳ�����ID:");
			scanf("%d", &id);
			Seat_UI_MgtEntry(id);
			paging.totalRecords = Studio_Srv_FetchAll(head);
			List_Paging(head, paging, studio_node_t);
			break;
		case 'p':
		case 'P':
			if (1 < Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, studio_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, studio_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
	//�ͷ�����ռ�
	List_Destroy(head, studio_node_t);

}

void Studio_UI_ListAll(void) {
	int i;
	char choice;

	studio_list_t head;
	studio_node_t *pos;
	Pagination_t paging;

	List_Init(head, studio_node_t);
	paging.offset = 0;
	paging.pageSize = STUDIO_PAGE_SIZE;

	//��������
	paging.totalRecords = Studio_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do {
		system("cls");
		printf("\n\n");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t\t\t\t\t      �ݳ����б� \n\n");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t   %-10s  %-25s  %-15s  %-15s  %-15s\n\n", "ID", "�ݳ�����", "����", "����", "��λ����");
		printf("\t\t========================================================================================\n\n");
		
		//��ʾ����
		for (i = 0, pos = (studio_node_t *) (paging.curPos); pos != head && i < paging.pageSize; i++) {
			printf("\t\t   %-10d  %-25s  %-15d  %-15d  %-15d\n", pos->data.id, pos->data.name, pos->data.rowsCount
															, pos->data.colsCount, pos->data.seatsCount);
			pos = pos->next;
		}
		printf("\n\t\t========================�ܼ�¼:%2d ========================= ҳ %2d/%2d====================\n\n"
					, paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
		printf("\n\t\t\t\t   [N]  ��һҳ        [P]  ��һҳ        [R]  ���� \t\t\n");
//		printf("\n\t\t\t����������Ҫ���ҵ��:");

		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
/*		case 's':
		case 'S':
			printf("Input the ID:");
			scanf("%d", &id);
			Seat_UI_MgtEntry(id);
			paging.totalRecords = Studio_Srv_FetchAll(head);
			List_Paging(head, paging, studio_node_t);
			break;
	*/	case 'p':
		case 'P':
			if (1 < Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, studio_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, studio_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
	//�ͷ�����ռ�
	List_Destroy(head, studio_node_t);
}

int Studio_UI_Add(void) {
	studio_t rec;
	int newRecCount = 0;
	char choice;

	do {
		system("cls");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("t\t\t\t\t\t            ����µ��ݳ���\n");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\n\t\t�ݳ�������:");
		fflush(stdin);
		//getchar();
		gets(rec.name);
		printf("\n\t\t�ݳ�����λ����:");
		scanf("%d", &(rec.rowsCount));
		printf("\n\t\t�ݳ�����λ����:");
		scanf("%d", &(rec.colsCount));
		rec.seatsCount = rec.rowsCount*rec.colsCount;
		printf("\t\t========================================================================================\n\n");

		//��ȡ����
		rec.id = EntKey_Srv_CompNewKey("Studio");

		if (Studio_Srv_Add(&rec)) {
			newRecCount += 1;
			printf("\n\t\t�µ��ݳ�����ӳɹ���!\n\n");
		} else
			printf("\n\t\t�ݳ������ʧ����\n\n");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\n\t\t\t\t   [A]  �������                         [R]  ����\n");
		fflush(stdin);
		getchar();

		printf("\t\t���ѡ�");
		scanf("%c", &choice);
	} while ('a' == choice || 'A' == choice);
	return newRecCount;
}

int Studio_UI_Modify(int id) {
	studio_t rec;
	int rtn = 0;
	int newrow, newcolumn;
	seat_list_t list;
	int seatcount;

	/*Load record*/
	if (!Studio_Srv_FetchByID(id, &rec)) {
		printf("\t\t�ݳ���������!\n\t\t��[Enter]������!\n");
		fflush(stdin);
		getchar();
		return 0;
	}
	system("cls");
	printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
	printf("\n\t\t\t\t\t\t�����ݳ�������\n");
	printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
	printf("\n\t\t��Ҫ�޸ĵ��ݳ�����ID:%d\n", rec.id);
	printf("\n\t\t�ݳ���������[%s]:", rec.name);
	fflush(stdin);
	getchar();
	gets(rec.name);

	List_Init(list, seat_node_t);
	seatcount = Seat_Srv_FetchByRoomID(list, rec.id);
	if(seatcount){
		do{			//�����λ�ļ���������λ��Ϣ������µ����б������ǰ�󣬷����������
			printf("\n\t\t���º����λ����[%d](���º����λ�������������ԭ����������)", rec.rowsCount);
			scanf("%d", &(newrow));
			printf("\n\t\t���º����λ����[%d](���º����λ�������������ԭ����������)", rec.colsCount);
			scanf("%d", &(newcolumn));
		}while(	newrow<rec.rowsCount||newcolumn<rec.colsCount);
		rec.rowsCount=newrow;
		rec.colsCount=newcolumn;
		rec.seatsCount=seatcount;
	}
	else{
		printf("\n\t\t��������º������:");
		scanf("%d", &rec.rowsCount);
		printf("\n\t\t��������º������:");
		scanf("%d", &rec.colsCount);
		rec.seatsCount=rec.rowsCount*rec.colsCount;
	}
	printf("\n\t\t----------------------------------------------------------------------------------------\n");

	if (Studio_Srv_Modify(&rec)) {
		rtn = 1;
		printf("\n\t\t�ݳ������ݸ��ĳɹ�!\n\t\t��[Enter]�����з���!\n");
	} else
		printf("\t\t\t�ݳ������ݸ���ʧ��!\n\t\t��[Enter]������!\n");

	fflush(stdin);
	printf("\t");
	getch();
	return rtn;
}

int Studio_UI_Delete(int id) {

	int rtn = 0;

	if (Studio_Srv_DeleteByID(id)) {
		//��ɾ����ӳ��ʱ��ͬʱ���ݷ�ӳ��idɾ����λ�ļ��е���λ
		printf("!!!!!!!!!!!!\n");
		if(Seat_Srv_DeleteAllByRoomID(id))
			printf("\n\t\tɾ���ɹ���\n");
			rtn = 1;
		} else {
			printf("\n\t\t���ݳ���������!\n");
		}
	fflush(stdin);
	getchar();
	printf("\t");
	system("pause");

	return rtn;
}

