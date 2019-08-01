


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
	 * Description: 显示所有的演出计划信息,进行查询。
	 * Input:       无
	 * Output:      所有剧目的信息
	 * Return:      无
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
		printf("\t\t\t\t\t\t      演出计划 \n\n");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t%-5s    %-5s        %-20s  %-13s  %-13s  %-5s\n", "计划ID", "放映厅ID", "剧目ID", "日期", "时间", "座位数量");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		for (i = 0, pos = (schedule_node_t *) (paging.curPos);pos != head && i < paging.pageSize; i++) {
			printf("%-5d  %-5d  %-5d  %-8d %d %d  %-8d %d %d  %-5d\n",
				pos->data.id, pos->data.studio_id, pos->data.id, pos->data.date.year, pos->data.date.month, pos->data.date.day,
				pos->data.time.hour, pos->data.time.minute, pos->data.time.second, pos->data.seat_count);
				pos = pos->next;
		}
		printf("\n\t\t========================总记录:%2d ========================= 页 %2d/%2d====================\n\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));

		printf("\n\t\t\t\t   [N]  下一页        [P]  上一页        [R]  返回 \t\t\n");
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
 * Description: 添加一条演出计划信息
 * Input:       相关剧目的ID号
 * Output:      输入信息时的各种提示
 * Return:      添加的记录数
 */
int Schedule_UI_Add(int play_id) {
	schedule_t rec;
	int newRecCount = 0;
	char choice;


	do {
		system("cls");
/*		printf("\n=======================================================\n");
		printf("****************       添加新演出计划         ****************\n");
		printf("-------------------------------------------------------\n");
*/
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t\t\t\t\t   添加新演出计划 \n\n");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");

		rec.id = EntKey_Srv_CompNewKey("Schedule");
		rec.play_id = play_id;
		printf("\t\t演出厅ID：");
		scanf("%d",&rec.studio_id);
		fflush(stdin);
		printf("\t\t座位数量：");
		scanf("%d",&rec.seat_count);
		fflush(stdin);
		printf("\t\t日期：");
		scanf("%d %d %d",&rec.date.year,&rec.date.month,&rec.date.day);
		fflush(stdin);
		printf("\t\t时间：");
		scanf("%d %d %d",&rec.time.hour,&rec.time.minute,&rec.time.second);

		printf("=======================================================\n");


		if (Schedule_Srv_Add(&rec)) {
				newRecCount += 1;
				printf("\t\t您已经成功添加该演出计划！\n");
				getchar();

			} else
				printf("\t\t添加演出计划失败！\n");
			printf("-------------------------------------------------------\n");
			printf("\t\t[A]添加更多\t \t [R]返回:");

			scanf("%c", &choice);
			fflush(stdin);
	} while ('a' == choice || 'A' == choice);

	return newRecCount;

}

/*
 * Function:    Schedule_UI_Modify
 * Function ID:	TTMS_SCU_Schedule_UI_Mod
 * Description: 按照ID号更新演出计划信息
 * Input:       待更新的演出计划ID号
 * Output:      输入信息时的各种提示
 * Return:      更新的剧目信息数，0表示未找到，1表示找到并更新
 */
int Schedule_UI_Modify(int id)
{
	schedule_t rec;
	int rtn = 0;

	if (!Schedule_Srv_FetchByID(id, &rec)) {
		printf("\t\t演出计划不存在\n按回车键返回\n");
		getchar();
		return 0;
	}

	printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
	printf("\t\t\t\t\t\t   更新演出计划 \n\n");
	printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
	printf("\n\t\t演出计划 ID:%d\n", rec.id);
	printf("\n\t\t剧目 ID:%d:\n", rec.play_id);

	printf("\n\t\t演出厅ID：%d\n",rec.studio_id);
	getchar();
	scanf("%d",&rec.studio_id);
	fflush(stdin);
	getchar();
	printf("\n\t\t座位数量：%d\n",rec.seat_count);
	scanf("%d",&rec.seat_count);

	printf("\n\t\t日期[%d %d %d]\n：",rec.date.year,rec.date.month,rec.date.day);
	scanf("%d %d %d",&rec.date.year,&rec.date.month,&rec.date.day);
	printf("\n\t\t时间[%d %d %d]：",rec.time.hour,rec.time.minute,rec.time.second);
	scanf("%d %d %d",&rec.time.hour,&rec.time.minute,&rec.time.second);

	fflush(stdin);

	printf("\n");
	printf("\n\t\t----------------------------------------------------------------------------------------\n\n");

	if (Schedule_Srv_Modify(&rec)) {
		rtn = 1;
		printf("\n\t\t演出计划更新成功!\n\t\t按[Enter]键返回!\n");
	} else
		printf("\n\t\t演出计划更新失败!\n\t\t按[Enter]键继续!\n");

	getchar();
	return rtn;

}

/*
 * Function:    Schedule_UI_Delete
 * Function ID:	TTMS_SCU_Schedule_UI_Del
 * Description: 按照ID号删除演出计划信息
 * Input:       待删除的剧目ID号
 * Output:      提示删除是否成功
 * Return:      0表示删除失败，1表示删除成功
 */
int Schedule_UI_Delete(int id){
	int rtn;
	schedule_t buf;
	if(Schedule_Srv_FetchByID(id,&buf)==0)
	{
		printf("\n\t\t未找到此演出计划！\n");
		rtn = 0;
	}
	else
	{
		if(Schedule_Srv_DeleteByID(id)==0)
		{
			printf("\n\t\t删除此演出计划失败！\n");
			rtn = 0;
		}
		else
		{
			printf("\n\t\t删除成功！\n");
			rtn = 1;
		}
	}
	getchar();
	return rtn;
}

/*
 * Function:    Schedule_UI_Query
 * Function ID:	TTMS_SCU_Schedule_UI_Qry
 * Description: 按照ID号查询演出计划信息
 * Input:       待查找的剧目ID号
 * Output:      查找到的剧目信息
 * Return:      0表示未找到，1表示找到了
 */
int Schedule_UI_Query(int id){
	int rtn;
	schedule_t buf;
	if(Schedule_Srv_FetchByID(id,&buf)==0)
	{
		printf("\t\t未找到此演出计划！\n");
		rtn = 0;
	}
	else
	{
		printf("\t\t已找到此剧目！\n");

		printf("\t\t%-5s    %-5s        %-20s  %-13s  %-13s  %-5s\n", "计划ID", "放映厅ID", "剧目ID", "日期", "时间", "座位数量");

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
 * Description: 以列表模式显示给定剧目的演出计划信息
 * Input:       list为查找到的演出计划信息链表的头指针，play为剧目信息，paging为显示分页设置参数
 * Output:      无
 * Return:      返回查找到的记录数目
 */
void Schedule_UI_ListByPlay(const play_t *play, schedule_list_t list, Pagination_t paging)
{

}

/*
 * Function:    Schedule_UI_MgtEntry
 * Function ID:	TTMS_SCU_Schedule_UI_MgtEnt
 * Description: 演出计划管理界面，演出计划按照剧目进行管理，以剧目ID号为输入
 * Input:       相关剧目ID号
 * Output:      演出计划信息
 * Return:      无
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
		printf("\t\t\t\t\t\t      演出计划 \n\n");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t%-5s    %-5s        %-20s  %-13s  %-13s  %-5s\n", "计划ID", "放映厅ID", "剧目ID", "日期", "时间", "座位数量");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		for (i = 0, pos = (schedule_node_t *) (paging.curPos);pos != head && i < paging.pageSize; i++){
			printf("\t\t%-8d  %-15d  %-15s   %4d年%02d月%02d日   %4d时%02d分%02d秒   %-5d\n",
				pos->data.id, pos->data.studio_id, buf.name, pos->data.date.year, pos->data.date.month, pos->data.date.day,
				pos->data.time.hour, pos->data.time.minute, pos->data.time.second, pos->data.seat_count);
				pos = pos->next;
		}
		printf("\n\t\t========================总记录:%2d ========================= 页 %2d/%2d====================\n\n",
			paging.totalRecords, Pageing_CurPage(paging),
			Pageing_TotalPages(paging));
		printf("\t\t[A] 添加计划 | [D] 删除计划 | [U] 更新计划 | [P] 上一页 | [N] 下一页 | [G] 出票 | [R] 返回");
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
			printf("\t\t请输入演出计划ID：");
			scanf("%d",&sch_id);
			fflush(stdin);
			if (Schedule_UI_Delete(sch_id)) {
				paging.totalRecords = Schedule_Srv_FetchByPlay(head, play_id);
				Paging_Locate_LastPage(head, paging, schedule_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("\t\t请输入演出计划ID：");
			scanf("%d",&sch_id);
			fflush(stdin);
			if (Schedule_UI_Modify(sch_id)) {
				paging.totalRecords = Schedule_Srv_FetchByPlay(head, play_id);
				Paging_Locate_LastPage(head, paging, schedule_node_t);
			}
			break;
		case 'g':
		case 'G':
			printf("\t\t请输入演出计划ID：");
			scanf("%d", &sch_id);
			if (Schedule_Srv_FetchByID(sch_id, &buf)) {
				Ticket_UI_MgtEntry(sch_id);
			} else {
				printf("\t\t演出计划不存在！\n");
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
