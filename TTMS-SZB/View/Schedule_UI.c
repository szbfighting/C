


#include "Schedule_UI.h"
#include "Play_UI.h"

#include "../Common/list.h"
#include "../Service/Schedule.h"
#include "../Service/Play.h"
#include "../Service/Studio.h"
#include "../Service/EntityKey.h"

#include <stdio.h>
#include <stdlib.h>

	/*
	 * Function:    Schedule_UI_ListAll
	 * Function ID:	TTMS_SCU_Schedule_UI_List
	 * Description: ��ʾ���е��ݳ��ƻ���Ϣ,���в�ѯ��
	 * Input:       ��
	 * Output:      ���о�Ŀ����Ϣ
	 * Return:      ��
	 */
void Schedule_UI_ListAll(void) {



	schedule_list_t head;
	List_Init(head, schedule_node_t);

	schedule_node_t* pos;
	int i;
	char choice;


	Pagination_t paging;
	paging.offset = 0;
	paging.pageSize = SCHEDULE_PAGE_SIZE;

	paging.totalRecords = Schedule_Srv_FetchAll(head);

	Paging_Locate_FirstPage(head, paging);


	do {

		system("cls");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t\t\t\t\t      �ݳ��ƻ� \n\n");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t%-5s    %-5s        %-20s  %-13s  %-13s  %-5s\n", "�ƻ�ID", "��ӳ��ID", "��ĿID", "����", "ʱ��", "��λ����");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		for (i = 0, pos = (schedule_node_t *) (paging.curPos);pos != head && i < paging.pageSize; i++) {
			printf("%-5d  %-5d  %-5d  %-8d %d %d  %-8d %d %d  %-5d\n",
				pos->data.id, pos->data.studio_id, pos->data.id, pos->data.date.year, pos->data.date.month, pos->data.date.day,
				pos->data.time.hour, pos->data.time.minute, pos->data.time.second, pos->data.seat_count);
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

/*
 * Function:    Schedule_UI_Add
 * Function ID:	TTMS_SCU_Schedule_UI_Add
 * Description: ���һ���ݳ��ƻ���Ϣ
 * Input:       ��ؾ�Ŀ��ID��
 * Output:      ������Ϣʱ�ĸ�����ʾ
 * Return:      ��ӵļ�¼��
 */
int Schedule_UI_Add(int play_id) {
	schedule_t rec;
	int newRecCount = 0;
	char choice;


	do {
		system("cls");
/*		printf("\n=======================================================\n");
		printf("****************       ������ݳ��ƻ�         ****************\n");
		printf("-------------------------------------------------------\n");
*/
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t\t\t\t\t   ������ݳ��ƻ� \n\n");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");

		rec.id = EntKey_Srv_CompNewKey("Schedule");
		rec.play_id = play_id;
		printf("\t\t�ݳ���ID��");
		scanf("%d",&rec.studio_id);
		fflush(stdin);
		printf("\t\t��λ������");
		scanf("%d",&rec.seat_count);
		fflush(stdin);
		printf("\t\t���ڣ�");
		scanf("%d %d %d",&rec.date.year,&rec.date.month,&rec.date.day);
		fflush(stdin);
		printf("\t\tʱ�䣺");
		scanf("%d %d %d",&rec.time.hour,&rec.time.minute,&rec.time.second);

		printf("=======================================================\n");


		if (Schedule_Srv_Add(&rec)) {
				newRecCount += 1;
				printf("\t\t���Ѿ��ɹ���Ӹ��ݳ��ƻ���\n");
				getchar();

			} else
				printf("\t\t����ݳ��ƻ�ʧ�ܣ�\n");
			printf("-------------------------------------------------------\n");
			printf("\t\t[A]��Ӹ���\t \t [R]����:");

			scanf("%c", &choice);
			fflush(stdin);
	} while ('a' == choice || 'A' == choice);

	return newRecCount;

}

/*
 * Function:    Schedule_UI_Modify
 * Function ID:	TTMS_SCU_Schedule_UI_Mod
 * Description: ����ID�Ÿ����ݳ��ƻ���Ϣ
 * Input:       �����µ��ݳ��ƻ�ID��
 * Output:      ������Ϣʱ�ĸ�����ʾ
 * Return:      ���µľ�Ŀ��Ϣ����0��ʾδ�ҵ���1��ʾ�ҵ�������
 */
int Schedule_UI_Modify(int id)
{
	schedule_t rec;
	int rtn = 0;

	if (!Schedule_Srv_FetchByID(id, &rec)) {
		printf("\t\t�ݳ��ƻ�������\n���س�������\n");
		getchar();
		return 0;
	}

	printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
	printf("\t\t\t\t\t\t   �����ݳ��ƻ� \n\n");
	printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
	printf("\n\t\t�ݳ��ƻ� ID:%d\n", rec.id);
	printf("\n\t\t��Ŀ ID:%d:\n", rec.play_id);

	printf("\n\t\t�ݳ���ID��%d\n",rec.studio_id);
	getchar();
	scanf("%d",&rec.studio_id);
	fflush(stdin);
	getchar();
	printf("\n\t\t��λ������%d\n",rec.seat_count);
	scanf("%d",&rec.seat_count);

	printf("\n\t\t����[%d %d %d]\n��",rec.date.year,rec.date.month,rec.date.day);
	scanf("%d %d %d",&rec.date.year,&rec.date.month,&rec.date.day);
	printf("\n\t\tʱ��[%d %d %d]��",rec.time.hour,rec.time.minute,rec.time.second);
	scanf("%d %d %d",&rec.time.hour,&rec.time.minute,&rec.time.second);

	fflush(stdin);

	printf("\n");
	printf("\n\t\t----------------------------------------------------------------------------------------\n\n");

	if (Schedule_Srv_Modify(&rec)) {
		rtn = 1;
		printf("\n\t\t�ݳ��ƻ����³ɹ�!\n\t\t��[Enter]������!\n");
	} else
		printf("\n\t\t�ݳ��ƻ�����ʧ��!\n\t\t��[Enter]������!\n");

	getchar();
	return rtn;

}

/*
 * Function:    Schedule_UI_Delete
 * Function ID:	TTMS_SCU_Schedule_UI_Del
 * Description: ����ID��ɾ���ݳ��ƻ���Ϣ
 * Input:       ��ɾ���ľ�ĿID��
 * Output:      ��ʾɾ���Ƿ�ɹ�
 * Return:      0��ʾɾ��ʧ�ܣ�1��ʾɾ���ɹ�
 */
int Schedule_UI_Delete(int id){
	int rtn;
	schedule_t buf;
	if(Schedule_Srv_FetchByID(id,&buf)==0)
	{
		printf("\n\t\tδ�ҵ����ݳ��ƻ���\n");
		rtn = 0;
	}
	else
	{
		if(Schedule_Srv_DeleteByID(id)==0)
		{
			printf("\n\t\tɾ�����ݳ��ƻ�ʧ�ܣ�\n");
			rtn = 0;
		}
		else
		{
			printf("\n\t\tɾ���ɹ���\n");
			rtn = 1;
		}
	}
	getchar();
	return rtn;
}

/*
 * Function:    Schedule_UI_Query
 * Function ID:	TTMS_SCU_Schedule_UI_Qry
 * Description: ����ID�Ų�ѯ�ݳ��ƻ���Ϣ
 * Input:       �����ҵľ�ĿID��
 * Output:      ���ҵ��ľ�Ŀ��Ϣ
 * Return:      0��ʾδ�ҵ���1��ʾ�ҵ���
 */
int Schedule_UI_Query(int id){
	int rtn;
	schedule_t buf;
	if(Schedule_Srv_FetchByID(id,&buf)==0)
	{
		printf("\t\tδ�ҵ����ݳ��ƻ���\n");
		rtn = 0;
	}
	else
	{
		printf("\t\t���ҵ��˾�Ŀ��\n");

		printf("\t\t%-5s    %-5s        %-20s  %-13s  %-13s  %-5s\n", "�ƻ�ID", "��ӳ��ID", "��ĿID", "����", "ʱ��", "��λ����");

		printf("\t\t%-5d  %-5d  %-5d  %-8d %d %d  %-8d %d %d  %-5d\n",
				buf.id, buf.studio_id,buf.play_id, buf.date.year, buf.date.month, buf.date.day,
				buf.time.hour, buf.time.minute, buf.time.second, buf.seat_count);
		rtn = 1;
	}

	getchar();
	return rtn;
}

/*
 * Function:    Schedule_UI_ListByPlay
 * Function ID:	TTMS_SCU_Schedule_UI_ListByPlay
 * Description: ���б�ģʽ��ʾ������Ŀ���ݳ��ƻ���Ϣ
 * Input:       listΪ���ҵ����ݳ��ƻ���Ϣ�����ͷָ�룬playΪ��Ŀ��Ϣ��pagingΪ��ʾ��ҳ���ò���
 * Output:      ��
 * Return:      ���ز��ҵ��ļ�¼��Ŀ
 */
void Schedule_UI_ListByPlay(const play_t *play, schedule_list_t list, Pagination_t paging)
{

}

/*
 * Function:    Schedule_UI_MgtEntry
 * Function ID:	TTMS_SCU_Schedule_UI_MgtEnt
 * Description: �ݳ��ƻ�������棬�ݳ��ƻ����վ�Ŀ���й����Ծ�ĿID��Ϊ����
 * Input:       ��ؾ�ĿID��
 * Output:      �ݳ��ƻ���Ϣ
 * Return:      ��
 */
void Schedule_UI_MgtEntry(int play_id){
	play_t buf;
	Play_Srv_FetchByID(play_id, &buf);

	schedule_list_t head;
	List_Init(head, schedule_node_t);

	schedule_node_t* pos;
	int i;
	char choice;


	Pagination_t paging;
	paging.offset = 0;
	paging.pageSize = SCHEDULE_PAGE_SIZE;

	paging.totalRecords = Schedule_Srv_FetchByPlay(head, play_id);

	Paging_Locate_FirstPage(head, paging);


	do {

		system("cls");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t\t\t\t\t      �ݳ��ƻ� \n\n");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t%-5s    %-5s        %-20s  %-13s  %-13s  %-5s\n", "�ƻ�ID", "��ӳ��ID", "��ĿID", "����", "ʱ��", "��λ����");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		for (i = 0, pos = (schedule_node_t *) (paging.curPos);pos != head && i < paging.pageSize; i++){
			printf("\t\t%-8d  %-15d  %-15s   %4d��%02d��%02d��   %4dʱ%02d��%02d��   %-5d\n",
				pos->data.id, pos->data.studio_id, buf.name, pos->data.date.year, pos->data.date.month, pos->data.date.day,
				pos->data.time.hour, pos->data.time.minute, pos->data.time.second, pos->data.seat_count);
				pos = pos->next;
		}
		printf("\n\t\t========================�ܼ�¼:%2d ========================= ҳ %2d/%2d====================\n\n",
			paging.totalRecords, Pageing_CurPage(paging),
			Pageing_TotalPages(paging));
		printf("\t\t[A] ��Ӽƻ� | [D] ɾ���ƻ� | [U] ���¼ƻ� | [P] ��һҳ | [N] ��һҳ | [G] ��Ʊ | [R] ����");
		printf("\n\t\t========================================================================================\n\n");
		printf("\t\tYour Choice:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		int sch_id;
		switch (choice){
		case 'a':
		case 'A':
			if (Schedule_UI_Add(play_id)){
				paging.totalRecords = Schedule_Srv_FetchByPlay(head, play_id);
				Paging_Locate_LastPage(head, paging, schedule_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("\t\t�������ݳ��ƻ�ID��");
			scanf("%d",&sch_id);
			fflush(stdin);
			if (Schedule_UI_Delete(sch_id)) {
				paging.totalRecords = Schedule_Srv_FetchByPlay(head, play_id);
				Paging_Locate_LastPage(head, paging, schedule_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("\t\t�������ݳ��ƻ�ID��");
			scanf("%d",&sch_id);
			fflush(stdin);
			if (Schedule_UI_Modify(sch_id)) {
				paging.totalRecords = Schedule_Srv_FetchByPlay(head, play_id);
				Paging_Locate_LastPage(head, paging, schedule_node_t);
			}
			break;
		case 'g':
		case 'G':
			printf("\t\t�������ݳ��ƻ�ID��");
			scanf("%d", &sch_id);
			if (Schedule_Srv_FetchByID(sch_id, &buf)) {
				Ticket_UI_MgtEntry(sch_id);
			} else {
				printf("\t\t�ݳ��ƻ������ڣ�\n");
			}
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
