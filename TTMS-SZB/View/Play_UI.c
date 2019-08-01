

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
 * Description: 以列表模式显示剧目信息
 * Input:       list剧目信息链表的头指针，paging分页设置参数
 * Output:      所有剧目的信息
 * Return:      返回查找到的记录数目
 */
void Play_UI_ShowList(play_list_t list, Pagination_t paging) {

	int i;
	play_node_t *pos;
	char choice;
	do {
			system("cls");
			printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
			printf("\t\t\t\t\t\t      剧目列表 \n\n");
			printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
			printf("\t\t%-5s  %-15s  %-10s  %-5s  %-5s  %-6s  %-6s  %-8s  %-12s\n\n", "ID", "剧目名", "区域",
				"时长", "票价", "剧目类型", "剧目级别", "上映时间", "下映时间");
			printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
			for (i = 0, pos = (play_node_t *) (paging.curPos); pos != list && i < paging.pageSize; i++)	{
				printf("%-5d  %-15s  %-10s  %-5d  %-5d",
						pos->data.id, pos->data.name, pos->data.area, pos->data.duration, pos->data.price);
				switch(pos->data.type){
				case PLAY_TYPE_FILE:
					printf("  %-8s","电影");
					break;
				case PLAY_TYPE_OPEAR:
					printf("  %-8s","歌剧");
					break;
				case PLAY_TYPE_CONCERT:
					printf("  %-8s","音乐会");
					break;
				}
				switch(pos->data.rating)
				{
				case PLAY_RATE_CHILD:
					printf("  %-8s","儿童");
					break;
				case PLAY_RATE_TEENAGE:
					printf("  %-8s","青少年");
					break;
				case PLAY_RATE_ADULT:
					printf("  %-8s","成人");
					break;
				}
				printf("  %d年%d月%d日 %d年%d月%d日\n",pos->data.start_date.year, pos->data.start_date.month, pos->data.start_date.day,
														pos->data.end_date.year, pos->data.start_date.month, pos->data.start_date.day);
				pos = pos->next;
				}
			printf("\n\t\t========================总记录:%2d ========================= 页 %2d/%2d====================\n\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
			printf("\n\t\t\t\t   [N]  下一页        [P]  上一页        [R]  返回 \t\t\n");
			printf("\t\t========================================================================================\n\n");
			printf("\t\t请选择选项:");
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
 * Description: 剧目信息管理界面
 * Input:       flag为0，进入管理页面，flag为1进入查询界面
 * Output:      剧目管理界面
 * Return:      无
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
			printf("\t\t\t\t\t\t      剧目列表 \n\n");
			printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
			printf("\t\t%-5s  %-10s  %-10s  %-5s  %-5s  %-6s  %-6s  %-8s  %-12s\n\n", "ID", "剧目名", "区域",
				"时长", "票价", "剧目类型", "剧目级别", "上映时间", "下映时间");
			printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
			for (i = 0, pos = (play_node_t *) (paging.curPos); pos != head && i < paging.pageSize; i++){
				printf("\t\t%-5d  %-10s  %-10s  %-5d  %-5d",
						pos->data.id, pos->data.name, pos->data.area, pos->data.duration, pos->data.price);

				switch(pos->data.type)
				{
				case PLAY_TYPE_FILE:
					printf("  %-8s","电影");
					break;
				case PLAY_TYPE_OPEAR:
					printf("  %-8s","歌剧");
					break;
				case PLAY_TYPE_CONCERT:
					printf("  %-8s","音乐会");
					break;
				}


				switch(pos->data.rating)
				{
				case PLAY_RATE_CHILD:
					printf("  %-8s","儿童");
					break;
					case PLAY_RATE_TEENAGE:
						printf("  %-8s","青少年");
						break;
				case PLAY_RATE_ADULT:
					printf("  %-8s","成人");
					break;
				}

						printf("%d年%d月%d日   %d年%d月%d日\n",pos->data.start_date.year, pos->data.start_date.month, pos->data.start_date.day,
						pos->data.end_date.year, pos->data.end_date.month, pos->data.end_date.day);

//				printf("%d年%d月%d日   %d年%d月%d日\n", pos->data.start_date.year, pos->data.start_date.month, pos->data.start_date.day,
//					pos->data.end_date.year, pos->data.end_date.month, pos->data.end_date.day);
				pos = pos->next;
			}
			printf("\n\t\t========================总记录:%2d ========================= 页 %2d/%2d====================\n\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
			printf("\t\t[P]上一页 | [N]下一页 | [A]添加剧目 | [D]删除剧目 | [U]更新剧目 | [S]演出计划 | [R]返 回");
			printf("\n\t\t========================================================================================\n\n");
			printf("\t\t请选择选项:");
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
				printf("\t\t请输入剧目ID：");
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
				printf("\t\t请输入剧目ID：");
				scanf("%d",&playID);
				fflush(stdin);
				if (Play_UI_Delete(playID)) {
					paging.totalRecords = Play_Srv_FetchAll(head);
					Paging_Locate_LastPage(head, paging, play_node_t);
				}
				break;
			case 'u':
			case 'U':
				printf("\t\t请输入剧目ID：");
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
 * Description: 添加一条剧目信息
 * Input:       无
 * Output:      输入信息时的各种提示
 * Return:      添加的记录数
 */
int Play_UI_Add(void) {
	play_t rec;
	int newRecCount = 0;
	char choice;


	do {
		system("cls");
		/*printf("\n=======================================================\n");
		printf("****************       添 加 新 剧 目         ****************\n");
		printf("-------------------------------------------------------\n");
*/
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t\t\t\t\t      添加剧目 \n\n");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");

		rec.id = EntKey_Srv_CompNewKey("Play");
		//getchar();
		printf("\n\t\t剧目名：");
		gets(rec.name);

		printf("\t\t剧目时长：");
		scanf("%d",&rec.duration);
		fflush(stdin);
		//getchar();
		printf("\t\t地区：");
		gets(rec.area);

		printf("\t\t票价：");
		scanf("%d",&rec.price);
		
		char typec;
		printf("\t\t剧目类型( 1:电影 2:歌剧 3:音乐会 ):");

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
		printf("\n\t\t限制级( 1:儿童 2:青少年 3:成人 ):");
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
		printf("\n\t\t开始时间：");
		scanf("%d %d %d",&rec.start_date.year,&rec.start_date.month,&rec.start_date.day);
		printf("\n\t\t结束时间：");
		scanf("%d %d %d",&rec.end_date.year,&rec.end_date.month,&rec.end_date.day);
		printf("\t\t========================================================================================\n\n");

		if (Play_Srv_Add(&rec)) {
			newRecCount += 1;
			printf("\n\t\t新剧目添加成功！!\n");
		} else
			printf("\n\t\t新剧目添加失败!\n");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\n\t\t[A]\t添加更多\t\t [R]\t返回:\n");
		scanf("%c", &choice);
		fflush(stdin);
	} while ('a' == choice || 'A' == choice);
	return newRecCount;
}

/*
 * Function:    Play_UI_Modify
 * Function ID:	TTMS_SCU_Play_UI_Mod
 * Description: 更新剧目信息
 * Input:       待更新的剧目ID号
 * Output:      输入信息时的各种提示
 * Return:      更新的剧目信息数，0表示未找到，1表示找到并更新
 */
int Play_UI_Modify(int id) {

	play_t rec;
	int rtn = 0;

	if (!Play_Srv_FetchByID(id, &rec)) {
		printf("\n\t\t剧目不存在\n\n\t按回车键返回\n");
		getchar();
		return 0;
	}

	printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
	printf("\t\t\t\t\t\t      更新剧目 \n\n");
	printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
	printf("\n\t\t剧目 ID:%d\n", rec.id);
	printf("\n\t\t剧目名[%s]:", rec.name);
	fflush(stdin);
	getchar();
	gets(rec.name);

	printf("\n\t\t剧目时[%d]:",rec.duration);
	scanf("%d",&rec.duration);
	fflush(stdin);
	getchar();
	printf("\n\t\t地区[%s]:",rec.area);
	gets(rec.area);

	printf("\n\t\t票价[%d]:",rec.price);
	scanf("%d",&rec.price);


	printf("\n\t\t剧目类型[");
	switch(rec.type)
	{
	case PLAY_TYPE_FILE:
		printf("电影]\n");
		break;
	case PLAY_TYPE_OPEAR:
		printf("歌剧]\n");
		break;
	case PLAY_TYPE_CONCERT:
		printf("音乐会]\n");
		break;
	}

	char typec;
	printf("\t\t剧目类型(1:电影 2:歌剧 3:音乐会):");
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

	printf("\n\t\t限制级[");
	switch(rec.rating)
	{
	case PLAY_RATE_CHILD:
		printf("儿童]\n");
		break;
	case PLAY_RATE_TEENAGE:
		printf("青少年]\n");
		break;
	case PLAY_RATE_ADULT:
		printf("成年]\n");
		break;
	}

	printf("\n\t\t剧目类型(1:儿童 2:青少年 3:成年):");
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


	printf("\t\t开始时间[%d %d %d]\n：",rec.start_date.year,rec.start_date.month,rec.start_date.day);
	scanf("%d %d %d",&rec.start_date.year,&rec.start_date.month,&rec.start_date.day);
	printf("\t\t结束时间[%d %d %d]：",rec.end_date.year,rec.end_date.month,rec.end_date.day);
	scanf("%d %d %d",&rec.end_date.year,&rec.end_date.month,&rec.end_date.day);

	fflush(stdin);

	printf("\n");
	printf("\n\t\t----------------------------------------------------------------------------------------\n\n");

	if (Play_Srv_Modify(&rec)) {
		rtn = 1;
		printf("\n\t\t剧目信息更新成功!\n\t按[Enter]键进行返回!\n");
	} else
		printf("\n\t\t剧目信息更新失败!\n\t按[Enter]键进行返回!\n");
	getchar();
	return rtn;

}

/*
 * Function:    Play_UI_Delete
 * Function ID:	TTMS_SCU_Play_UI_Del
 * Description: 按照ID号删除剧目信息
 * Input:       待删除的剧目ID号
 * Output:      提示删除是否成功
 * Return:      0表示删除失败，1表示删除成功
 */
int Play_UI_Delete(int id) {
	int rtn;
	play_t buf;
	if(Play_Srv_FetchByID(id,&buf)==0)
	{
		printf("\n\t\t未找到此剧目！\n");
		getchar();
		rtn = 0;
	}
	else
	{
		if(Play_Srv_DeleteByID(id)==0)
		{
			printf("\n\t\t删除此剧目失败！\n");
			getchar();
			rtn = 0;
		}
		else
		{
			printf("\n\t\t删除成功！\n");
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
 * Description: 按照ID号查询剧目信息
 * Input:       待查找的剧目ID号
 * Output:      查找到的剧目信息
 * Return:      0表示未找到，1表示找到了
 */
int Play_UI_Query(int id)
{
	int rtn;
	play_t buf;
	if(Play_Srv_FetchByID(id,&buf)==0)
	{
		printf("\n\t\t未找到此剧目！\n");
		rtn = 0;
	}
	else
	{
		printf("\n\t\t已找到此剧目！\n");
		printf("\t\t%-5s  %-15s  %-10s  %-5s  %-5s  %-8s  %-8s  %-8s  %-12s\n", "ID", "剧目名", "区域",
									"时长", "票价", "剧目类型","剧目级别","上映时间", "下映时间");
		printf("\t\t%-5d  %-15s  %-10s  %-5d  %-5d",
										buf.id, buf.name, buf.area, buf.duration, buf.price);

		switch(buf.type)
		{
			case PLAY_TYPE_FILE:
				printf("  %-8s","电影");
				break;
			case PLAY_TYPE_OPEAR:
				printf("  %-8s","歌剧");
				break;
			case PLAY_TYPE_CONCERT:
				printf("  %-8s","音乐会");
				break;
		}


		switch(buf.rating)
		{
		case PLAY_RATE_CHILD:
			printf("  %-8s","儿童");
			break;
		case PLAY_RATE_TEENAGE:
			printf("  %-8s","青少年");
			break;
		case PLAY_RATE_ADULT:
			printf("  %-8s","成人");
			break;
		}

		printf("\t\t  %-4d %-2d %-2d  %-6d %-2d %-2d\n",buf.start_date.year, buf.start_date.month, buf.start_date.day,
											buf.end_date.year, buf.start_date.month, buf.start_date.day);

		rtn = 1;
	}

	getchar();
	return rtn;
}
