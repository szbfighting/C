

#include "Schedule.h"
#include "../Common/list.h"
#include "../Persistence/Schedule_Persist.h"
#include "Ticket.h"
#include <stdio.h>

/*
 * Function:    Schedule_Srv_Add
 * Function ID:	TTMS_SCU_Schedule_Srv_Add
 * Description: 添加一条演出计划信息
 * Input:       待添加的演出计划信息数据
 * Output:      无
 * Return:      添加的记录数
 */
int Schedule_Srv_Add(const schedule_t *data) {
	int rtn =  Schedule_Perst_Insert(data);
//	printf("****srvs\n");getchar();
	Ticket_Srv_AddBatch(data->id,data->studio_id);
	return rtn;
}

/*
 * Function:    Schedule_Srv_Modify
 * Function ID:	TTMS_SCU_Schedule_ Srv _Mod
 * Description: 更新一条演出计划信息
 * Input:       待更新的演出计划信息数据
 * Output:      无
 * Return:      更新的演出计划信息数，0表示未找到，1表示找到并更新
 */
int Schedule_Srv_Modify(const schedule_t *data) {
	int rtn =  Schedule_Perst_Update(data);
	Ticket_Srv_DeleteBatch(data->id);
	Ticket_Srv_AddBatch(data->id,data->studio_id);
	return rtn;
}

/*
 * Function:    Schedule_Srv_DeleteByID
 * Function ID:	TTMS_SCU_Schedule_Srv_DelByID
 * Description: 按照ID号删除演出计划信息
 * Input:       待删除的演出计划ID号
 * Output:      无
 * Return:      0表示删除失败，1表示删除成功
 */
int Schedule_Srv_DeleteByID(int ID) {
	int rtn =  Schedule_Perst_DeleteByID(ID);
	Ticket_Srv_DeleteBatch(ID);
	return rtn;
}

/*
 * Function:    Schedule_Srv_FetchByID
 * Function ID:	TTMS_SCU_Schedule_Srv_FetchByID
 * Description: 按照ID号查找一条演出计划信息，存入buf所指单元
 * Input:       待查找的演出计划ID号，将查找到的信息保存到buf中
 * Output:      无
 * Return:      0表示未找到，1表示找到了
 */
int Schedule_Srv_FetchByID(int ID, schedule_t *buf) {
	return Schedule_Perst_SelectByID(ID,buf);

}

/*
 * Function:    Schedule_Srv_FetchAll
 * Function ID:	TTMS_SCU_Schedule_Srv_FetchAll
 * Description: 取出所有演出计划信息，串成一条链表
 * Input:       list演出计划信息链表的头指针
 * Output:      无
 * Return:      返回查找到的记录数目
 */
int Schedule_Srv_FetchAll(schedule_list_t list) {
	return Schedule_Perst_SelectAll(list);
}

/*
 * Function:    Schedule_Srv_FetchByPlay
 * Function ID:	TTMS_SCU_Schedule_Srv_FetchByPlay
 * Description: 按照剧目ID号，将相关演出计划信息取出，串成一条链表
 * Input:       list为查找到的演出计划信息链表的头指针，play_id为剧目的ID号
 * Output:      无
 * Return:      返回查找到的记录数目
 */
int Schedule_Srv_FetchByPlay(schedule_list_t list,int play_id) {
	return Schedule_Perst_SelectByPlay(list,play_id);
}

/*
 * Function:    Schedule_Srv_StatRevByPlay
 * Function ID:	TTMS_SCU_Schedule_Srv_StatByPlay
 * Description: 根据剧目ID，统计上座率及票房，返回票房数收入
 * Input:       play_id为待统计剧目的ID号，soldCount为卖出的票数，totalCount为总票数
 * Output:      无
 * Return:      返回票房收入
 */
int Schedule_Srv_StatRevByPlay(int play_id, int *soldCount, int *totalCount)
{
	*soldCount=0;
	*totalCount=0;
	int piaofang=0;
	schedule_list_t list;
//	ticket_list_t tlist;
	schedule_node_t* curPos;
//	ticket_node_t* p;
	List_Init(list,schedule_node_t);
//	List_Init(tlist,ticket_node_t);
	Schedule_Perst_SelectByPlay(list, play_id);

	List_ForEach(list, curPos)
	{
		/*Ticket_Perst_SelectBySchID(tlist, curPos->data.id);
		List_ForEach(tlist,p)
		{
			if(p->data.status==TICKET_SOLD)
			{
				*soldCount=*soldCount+1;
				piaofang+=(p->data.price);
			}
			*totalCount=*totalCount+1;
		}*/
		piaofang+=Ticket_Srv_StatRevBySchID(curPos->data.id, soldCount, totalCount);
	}

	return piaofang;



}

