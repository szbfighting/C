

#include "Play_UI.h"
#include "Schedule_UI.h"
#include "Ticket_UI.h"

#include "../Common/list.h"
#include "../Service/play.h"
#include "../Service/EntityKey.h"

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

/*
 * Function:    Play_UI_ShowList
 * Function ID:	TTMS_SCU_Play_UI_Show
 * Description: ���б�ģʽ��ʾ��Ŀ��Ϣ
 * Input:       list��Ŀ��Ϣ�����ͷָ�룬paging��ҳ���ò���
 * Output:      ���о�Ŀ����Ϣ
 * Return:      ���ز��ҵ��ļ�¼��Ŀ
 */
void Play_UI_ShowList(play_list_t list, Pagination_t paging) {

	int i;
	play_node_t *pos;
	char choice;
	do {
			system("cls");
			printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
			printf("\t\t\t\t\t\t      ��Ŀ�б� \n\n");
			printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
			printf("\t\t%-5s  %-15s  %-10s  %-5s  %-5s  %-6s  %-6s  %-8s  %-12s\n\n", "ID", "��Ŀ��", "����",
				"ʱ��", "Ʊ��", "��Ŀ����", "��Ŀ����", "��ӳʱ��", "��ӳʱ��");
			printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
			for (i = 0, pos = (play_node_t *) (paging.curPos); pos != list && i < paging.pageSize; i++)	{
				printf("%-5d  %-15s  %-10s  %-5d  %-5d",
						pos->data.id, pos->data.name, pos->data.area, pos->data.duration, pos->data.price);
				switch(pos->data.type){
				case PLAY_TYPE_FILE:
					printf("  %-8s","��Ӱ");
					break;
				case PLAY_TYPE_OPEAR:
					printf("  %-8s","���");
					break;
				case PLAY_TYPE_CONCERT:
					printf("  %-8s","���ֻ�");
					break;
				}
				switch(pos->data.rating)
				{
				case PLAY_RATE_CHILD:
					printf("  %-8s","��ͯ");
					break;
				case PLAY_RATE_TEENAGE:
					printf("  %-8s","������");
					break;
				case PLAY_RATE_ADULT:
					printf("  %-8s","����");
					break;
				}
				printf("  %d��%d��%d�� %d��%d��%d��\n",pos->data.start_date.year, pos->data.start_date.month, pos->data.start_date.day,
														pos->data.end_date.year, pos->data.start_date.month, pos->data.start_date.day);
				pos = pos->next;
				}
			printf("\n\t\t========================�ܼ�¼:%2d ========================= ҳ %2d/%2d====================\n\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
			printf("\n\t\t\t\t   [N]  ��һҳ        [P]  ��һҳ        [R]  ���� \t\t\n");
			printf("\t\t========================================================================================\n\n");
			printf("\t\t��ѡ��ѡ��:");
			fflush(stdin);
			scanf("%c", &choice);
			fflush(stdin);

			switch (choice)
			{
			case 'p':
			case 'P':
				if (1 < Pageing_CurPage(paging)) {
					Paging_Locate_OffsetPage(list, paging, -1, play_node_t);
				}
				break;
			case 'n':
			case 'N':
				if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
					Paging_Locate_OffsetPage(list, paging, 1, play_node_t);
				}
				break;
			}




	} while (choice != 'r' && choice != 'R');


}

/*
 * Function:    Play_UI_MgtEntry
 * Function ID:	TTMS_SCU_Play_UI_MgtEnt
 * Description: ��Ŀ��Ϣ�������
 * Input:       flagΪ0���������ҳ�棬flagΪ1�����ѯ����
 * Output:      ��Ŀ�������
 * Return:      ��
 */
void Play_UI_MgtEntry(int flag) {
	if(flag==0)
	{

		play_list_t head;
			List_Init(head, play_node_t);

			Pagination_t paging;

			paging.offset = 0;
			paging.pageSize = PLAY_PAGE_SIZE;
			paging.totalRecords = Play_Srv_FetchAll(head);
			Paging_Locate_FirstPage(head, paging);

			char choice;
			play_node_t* pos;
			int i;

	do {

			system("cls");
			printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
			printf("\t\t\t\t\t\t      ��Ŀ�б� \n\n");
			printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
			printf("\t\t%-5s  %-10s  %-10s  %-5s  %-5s  %-6s  %-6s  %-8s  %-12s\n\n", "ID", "��Ŀ��", "����",
				"ʱ��", "Ʊ��", "��Ŀ����", "��Ŀ����", "��ӳʱ��", "��ӳʱ��");
			printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
			for (i = 0, pos = (play_node_t *) (paging.curPos); pos != head && i < paging.pageSize; i++){
				printf("\t\t%-5d  %-10s  %-10s  %-5d  %-5d",
						pos->data.id, pos->data.name, pos->data.area, pos->data.duration, pos->data.price);

				switch(pos->data.type)
				{
				case PLAY_TYPE_FILE:
					printf("  %-8s","��Ӱ");
					break;
				case PLAY_TYPE_OPEAR:
					printf("  %-8s","���");
					break;
				case PLAY_TYPE_CONCERT:
					printf("  %-8s","���ֻ�");
					break;
				}


				switch(pos->data.rating)
				{
				case PLAY_RATE_CHILD:
					printf("  %-8s","��ͯ");
					break;
					case PLAY_RATE_TEENAGE:
						printf("  %-8s","������");
						break;
				case PLAY_RATE_ADULT:
					printf("  %-8s","����");
					break;
				}

						printf("%d��%d��%d��   %d��%d��%d��\n",pos->data.start_date.year, pos->data.start_date.month, pos->data.start_date.day,
						pos->data.end_date.year, pos->data.end_date.month, pos->data.end_date.day);

//				printf("%d��%d��%d��   %d��%d��%d��\n", pos->data.start_date.year, pos->data.start_date.month, pos->data.start_date.day,
//					pos->data.end_date.year, pos->data.end_date.month, pos->data.end_date.day);
				pos = pos->next;
			}
			printf("\n\t\t========================�ܼ�¼:%2d ========================= ҳ %2d/%2d====================\n\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
			printf("\t\t[P]��һҳ | [N]��һҳ | [A]��Ӿ�Ŀ | [D]ɾ����Ŀ | [U]���¾�Ŀ | [S]�ݳ��ƻ� | [R]�� ��");
			printf("\n\t\t========================================================================================\n\n");
			printf("\t\t��ѡ��ѡ��:");
			fflush(stdin);
			scanf("%c", &choice);
			fflush(stdin);

			int scheduleID;
			schedule_t buf;
			
			int playID;
			switch (choice)
			{
			case 's':
			case 'S':
				printf("\t\t�������ĿID��");
				scanf("%d",&playID);
				fflush(stdin);
				Schedule_UI_MgtEntry(playID);				
				break;
			case 'a':
			case 'A':
				if (Play_UI_Add()) {
					paging.totalRecords = Play_Srv_FetchAll(head);
					Paging_Locate_LastPage(head, paging, play_node_t);
				}
				break;
			case 'd':
			case 'D':
				printf("\t\t�������ĿID��");
				scanf("%d",&playID);
				fflush(stdin);
				if (Play_UI_Delete(playID)) {
					paging.totalRecords = Play_Srv_FetchAll(head);
					Paging_Locate_LastPage(head, paging, play_node_t);
				}
				break;
			case 'u':
			case 'U':
				printf("\t\t�������ĿID��");
				scanf("%d",&playID);
				fflush(stdin);
				if (Play_UI_Modify(playID)) {
					paging.totalRecords = Play_Srv_FetchAll(head);
					Paging_Locate_LastPage(head, paging, play_node_t);
				}
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
			else
			{
				DisplayQueryPlay();
			}
}

/*
 * Function:    Play_UI_Add
 * Function ID:	TTMS_SCU_Play_UI_Add
 * Description: ���һ����Ŀ��Ϣ
 * Input:       ��
 * Output:      ������Ϣʱ�ĸ�����ʾ
 * Return:      ��ӵļ�¼��
 */
int Play_UI_Add(void) {
	play_t rec;
	int newRecCount = 0;
	char choice;


	do {
		system("cls");
		/*printf("\n=======================================================\n");
		printf("****************       �� �� �� �� Ŀ         ****************\n");
		printf("-------------------------------------------------------\n");
*/
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t\t\t\t\t      ��Ӿ�Ŀ \n\n");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");

		rec.id = EntKey_Srv_CompNewKey("Play");
		//getchar();
		printf("\n\t\t��Ŀ����");
		gets(rec.name);

		printf("\t\t��Ŀʱ����");
		scanf("%d",&rec.duration);
		fflush(stdin);
		//getchar();
		printf("\t\t������");
		gets(rec.area);

		printf("\t\tƱ�ۣ�");
		scanf("%d",&rec.price);
		
		char typec;
		printf("\t\t��Ŀ����( 1:��Ӱ 2:��� 3:���ֻ� ):");

		do
		{
			typec = getchar();
		} while (typec!='1'&&typec!='2'&&typec!='3');
		putchar(typec);
		getchar();

		switch(typec)
		{
		case '1':
			rec.type = PLAY_TYPE_FILE;
			break;
		case '2':
			rec.type = PLAY_TYPE_OPEAR;
			break;
		case '3':
			rec.type = PLAY_TYPE_CONCERT;
			break;
		}
		printf("\n\t\t���Ƽ�( 1:��ͯ 2:������ 3:���� ):");
		do
		{
			typec = getchar();
		} while (typec!='1'&&typec!='2'&&typec!='3');
		putchar(typec);
		getchar();
		switch(typec)
		{
		case '1':
			rec.rating = PLAY_RATE_CHILD;
			break;
		case '2':
			rec.rating = PLAY_RATE_TEENAGE;
			break;
		case '3':
			rec.rating = PLAY_RATE_ADULT;
			break;
		}
		printf("\n\t\t��ʼʱ�䣺");
		scanf("%d %d %d",&rec.start_date.year,&rec.start_date.month,&rec.start_date.day);
		printf("\n\t\t����ʱ�䣺");
		scanf("%d %d %d",&rec.end_date.year,&rec.end_date.month,&rec.end_date.day);
		printf("\t\t========================================================================================\n\n");

		if (Play_Srv_Add(&rec)) {
			newRecCount += 1;
			printf("\n\t\t�¾�Ŀ��ӳɹ���!\n");
		} else
			printf("\n\t\t�¾�Ŀ���ʧ��!\n");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\n\t\t[A]\t��Ӹ���\t\t [R]\t����:\n");
		scanf("%c", &choice);
		fflush(stdin);
	} while ('a' == choice || 'A' == choice);
	return newRecCount;
}

/*
 * Function:    Play_UI_Modify
 * Function ID:	TTMS_SCU_Play_UI_Mod
 * Description: ���¾�Ŀ��Ϣ
 * Input:       �����µľ�ĿID��
 * Output:      ������Ϣʱ�ĸ�����ʾ
 * Return:      ���µľ�Ŀ��Ϣ����0��ʾδ�ҵ���1��ʾ�ҵ�������
 */
int Play_UI_Modify(int id) {

	play_t rec;
	int rtn = 0;

	if (!Play_Srv_FetchByID(id, &rec)) {
		printf("\n\t\t��Ŀ������\n\n\t���س�������\n");
		getchar();
		return 0;
	}

	printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
	printf("\t\t\t\t\t\t      ���¾�Ŀ \n\n");
	printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
	printf("\n\t\t��Ŀ ID:%d\n", rec.id);
	printf("\n\t\t��Ŀ��[%s]:", rec.name);
	fflush(stdin);
	getchar();
	gets(rec.name);

	printf("\n\t\t��Ŀʱ[%d]:",rec.duration);
	scanf("%d",&rec.duration);
	fflush(stdin);
	getchar();
	printf("\n\t\t����[%s]:",rec.area);
	gets(rec.area);

	printf("\n\t\tƱ��[%d]:",rec.price);
	scanf("%d",&rec.price);


	printf("\n\t\t��Ŀ����[");
	switch(rec.type)
	{
	case PLAY_TYPE_FILE:
		printf("��Ӱ]\n");
		break;
	case PLAY_TYPE_OPEAR:
		printf("���]\n");
		break;
	case PLAY_TYPE_CONCERT:
		printf("���ֻ�]\n");
		break;
	}

	char typec;
	printf("\t\t��Ŀ����(1:��Ӱ 2:��� 3:���ֻ�):");
	do{
		typec = getchar();

	} while (typec!='1'&&typec!='2'&&typec!='3');
	putchar(typec);
	getchar();
	switch(typec)
	{
	case '1':
		rec.type = PLAY_TYPE_FILE;
		break;
	case '2':
		rec.type = PLAY_TYPE_OPEAR;
		break;
	case '3':
		rec.type = PLAY_TYPE_CONCERT;
		break;
	}
	printf("\n");

	printf("\n\t\t���Ƽ�[");
	switch(rec.rating)
	{
	case PLAY_RATE_CHILD:
		printf("��ͯ]\n");
		break;
	case PLAY_RATE_TEENAGE:
		printf("������]\n");
		break;
	case PLAY_RATE_ADULT:
		printf("����]\n");
		break;
	}

	printf("\n\t\t��Ŀ����(1:��ͯ 2:������ 3:����):");
	do{
		typec = getchar();
	} while (typec!='1'&&typec!='2'&&typec!='3');
	putchar(typec);
	getchar();
	printf("\n");
	switch(typec)
	{
	case '1':
		rec.rating = PLAY_RATE_CHILD;
		break;
	case '2':
		rec.rating = PLAY_RATE_TEENAGE;
		break;
	case '3':
		rec.rating = PLAY_RATE_ADULT;
		break;
	}


	printf("\t\t��ʼʱ��[%d %d %d]\n��",rec.start_date.year,rec.start_date.month,rec.start_date.day);
	scanf("%d %d %d",&rec.start_date.year,&rec.start_date.month,&rec.start_date.day);
	printf("\t\t����ʱ��[%d %d %d]��",rec.end_date.year,rec.end_date.month,rec.end_date.day);
	scanf("%d %d %d",&rec.end_date.year,&rec.end_date.month,&rec.end_date.day);

	fflush(stdin);

	printf("\n");
	printf("\n\t\t----------------------------------------------------------------------------------------\n\n");

	if (Play_Srv_Modify(&rec)) {
		rtn = 1;
		printf("\n\t\t��Ŀ��Ϣ���³ɹ�!\n\t��[Enter]�����з���!\n");
	} else
		printf("\n\t\t��Ŀ��Ϣ����ʧ��!\n\t��[Enter]�����з���!\n");
	getchar();
	return rtn;

}

/*
 * Function:    Play_UI_Delete
 * Function ID:	TTMS_SCU_Play_UI_Del
 * Description: ����ID��ɾ����Ŀ��Ϣ
 * Input:       ��ɾ���ľ�ĿID��
 * Output:      ��ʾɾ���Ƿ�ɹ�
 * Return:      0��ʾɾ��ʧ�ܣ�1��ʾɾ���ɹ�
 */
int Play_UI_Delete(int id) {
	int rtn;
	play_t buf;
	if(Play_Srv_FetchByID(id,&buf)==0)
	{
		printf("\n\t\tδ�ҵ��˾�Ŀ��\n");
		getchar();
		rtn = 0;
	}
	else
	{
		if(Play_Srv_DeleteByID(id)==0)
		{
			printf("\n\t\tɾ���˾�Ŀʧ�ܣ�\n");
			getchar();
			rtn = 0;
		}
		else
		{
			printf("\n\t\tɾ���ɹ���\n");
			getchar();
			rtn = 1;
		}
	}
	getchar();
	return rtn;
}

/*
 * Function:    Play_UI_Query
 * Function ID:	TTMS_SCU_Play_UI_Qry
 * Description: ����ID�Ų�ѯ��Ŀ��Ϣ
 * Input:       �����ҵľ�ĿID��
 * Output:      ���ҵ��ľ�Ŀ��Ϣ
 * Return:      0��ʾδ�ҵ���1��ʾ�ҵ���
 */
int Play_UI_Query(int id)
{
	int rtn;
	play_t buf;
	if(Play_Srv_FetchByID(id,&buf)==0)
	{
		printf("\n\t\tδ�ҵ��˾�Ŀ��\n");
		rtn = 0;
	}
	else
	{
		printf("\n\t\t���ҵ��˾�Ŀ��\n");
		printf("\t\t%-5s  %-15s  %-10s  %-5s  %-5s  %-8s  %-8s  %-8s  %-12s\n", "ID", "��Ŀ��", "����",
									"ʱ��", "Ʊ��", "��Ŀ����","��Ŀ����","��ӳʱ��", "��ӳʱ��");
		printf("\t\t%-5d  %-15s  %-10s  %-5d  %-5d",
										buf.id, buf.name, buf.area, buf.duration, buf.price);

		switch(buf.type)
		{
			case PLAY_TYPE_FILE:
				printf("  %-8s","��Ӱ");
				break;
			case PLAY_TYPE_OPEAR:
				printf("  %-8s","���");
				break;
			case PLAY_TYPE_CONCERT:
				printf("  %-8s","���ֻ�");
				break;
		}


		switch(buf.rating)
		{
		case PLAY_RATE_CHILD:
			printf("  %-8s","��ͯ");
			break;
		case PLAY_RATE_TEENAGE:
			printf("  %-8s","������");
			break;
		case PLAY_RATE_ADULT:
			printf("  %-8s","����");
			break;
		}

		printf("\t\t  %-4d %-2d %-2d  %-6d %-2d %-2d\n",buf.start_date.year, buf.start_date.month, buf.start_date.day,
											buf.end_date.year, buf.start_date.month, buf.start_date.day);

		rtn = 1;
	}

	getchar();
	return rtn;
}
