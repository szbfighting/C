#include "Query_Play_UI.h"
#include "../Service/play.h"
#include "../Persistence/Query_Persist.h"
#include "../Service/schedule.h"
#include "../Persistence/Schedule_Persist.h"
#include "../Service/studio.h"
#include "../Persistence/Ticket_Persist.h"
#include "../View/Sale_UI.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void DisplayQueryPlay(void)
{
	fflush(stdin);
	char fileName[30];
	play_t play;

	char choice;
	do {
		fflush(stdin);
		memset(fileName, 0, sizeof(fileName));
		printf("\t\t�������Ŀ����:");
		scanf("%s",fileName);
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t\t\t\t\t      ��Ŀ��Ϣ�� \n\n");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		if (Query_PlayName(fileName,&play)) {
			printf("\t\t ID       ��Ŀ��       ����       ����         ӳ����        ��ӳʱ��         Ʊ��\n");
			printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
			printf("\t\t%d       %s     %s\t%s\t%d-%d-%d\t%d-%d-%d\t%d\n", play.id,
								play.name,	play.area,
								(play.rating==1?"child   ":(play.rating==2?"teenager":"adult   ")),
								play.start_date.year,play.start_date.month,
								play.start_date.day,play.end_date.year,
								play.end_date.month,play.end_date.day,play.price);
			schedule_t  sched;
			studio_t studio = {0};
	//		seat_list_t list, int studio_id
			if (Query_Schedule_By_PlayId(play.id, &sched)) {
				Query_Studio_By_Schedule(sched.studio_id, &studio);
				printf("\t\t�ⲿ��Ŀ���� %d:%d:%d %d:%d �� %s ��ӳ\n", sched.date.year, sched.date.month,
																			sched.date.day,sched.time.hour,
																			sched.time.minute, studio.name);
				printf("\t\t�ܹ�����λ %d , ������ %d, �� %d ����λ���ڳ���\n", Seat_Number_Count(sched.studio_id), Sold_Ticket_Count(sched.id),
																			Seat_Number_Count(sched.studio_id)-Sold_Ticket_Count(sched.id));
			} else {
				printf("\t\tsorry, the play is not show!\n");
			}
			printf("\t\t[R]eturn              [B]ooking              [A]gain:");
			fflush(stdin);
			//getchar();
			scanf("%c", &choice);
			switch(choice) {
			case 'a':
			case 'A':
				break;
			case 'B':
			case 'b':
				Sale_UI_ShowScheduler(play.id);
				return ;
			}
		} else {
			printf("\t\tthe play name is not exit!\n");
			printf("\t\t[R]eturn                              [A]gain:");
			fflush(stdin);
			getchar();
			scanf("%c", &choice);
			switch(choice) {
			case 'a':
			case 'A':
				break;

			}
		}
	} while (choice != 'r' && choice != 'R');
}


