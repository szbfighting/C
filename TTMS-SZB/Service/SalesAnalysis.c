
#include <string.h>
#include <stdio.h>
#include "SalesAnalysis.h"
#include "../Persistence/Sale_Persist.h"
#include "../Persistence/SalesAnalysis_Persist.h"
#include "Sale.h"
#include "Ticket.h"
#include "Play.h"
#include "Schedule.h"
#include "../Common/list.h"

//计算员工usrID在给定时间区间的销售额，返回销售额
int Salesanalysis_Srv_CompSaleVal(int usrID, user_date_t stDate, user_date_t endDate)
{
	int amount = 0;

	sale_list_t saleList;
	sale_node_t* pSale;
	List_Init(saleList, sale_node_t);
	amount=0;
	Sale_Perst_SelectByUsrID(saleList, usrID, stDate, endDate);
	List_ForEach(saleList, pSale)
	{

		amount += pSale->data.value;
	}

	List_Destroy(saleList, sale_node_t);

	return amount;
}

//结合剧目Play.dat,统计销售数据，构建销售分析salesanalysis_list_t list链表，返回list链表长度
int Salesanalysis_Srv_StaticSale(salesanalysis_list_t list)
{

	assert(list != NULL);

	play_list_t playList;
	play_node_t* pos;
	salesanalysis_node_t* newNode = (salesanalysis_node_t*)malloc(sizeof(salesanalysis_node_t));
	int sold, total;
	int count = 0;
	List_Free(list, salesanalysis_node_t);
	List_Init(playList, play_node_t);
	Play_Srv_FetchAll(playList);
	List_ForEach(playList, pos)
	{
		count++;

		newNode->data.sales = Schedule_Srv_StatRevByPlay(pos->data.id, &sold, &total);
		newNode->data.totaltickets = sold;
		newNode->data.start_date = pos->data.start_date;
		newNode->data.end_date = pos->data.end_date;
		strcpy(newNode->data.area,pos->data.area);
		strcpy(newNode->data.name,pos->data.name);
		newNode->data.price = pos->data.price;
		newNode->data.play_id = pos->data.id;
		newNode->data.price = pos->data.price;

		List_AddTail(list, newNode);

	}

	List_Destroy(playList,play_node_t);

	return count;
}

//对剧院电影票房排序
void SalesAnalysis_Srv_SortBySale(salesanalysis_list_t list)
{
	salesanalysis_node_t *p, *listLeft;
	assert(list!=NULL);

	if(List_IsEmpty(list))
	{
		return ;
	}

	list->prev->next = NULL;
	listLeft = list->next;
	list->next = list->prev = list;

	while (listLeft != NULL )
	{
		p = listLeft;
		listLeft = listLeft->next;

		SalesAnalysis_Srv_AddToSoftedList(list, p);

	}

}

void SalesAnalysis_Srv_AddToSoftedList(salesanalysis_list_t list, salesanalysis_node_t *node)
{

	salesanalysis_node_t *p;

	assert(list!=NULL && node!=NULL);

	if(List_IsEmpty(list))
	{
		List_AddTail(list, node);
	}
	else
	{
		//寻找插入位置
		p=list->next;

		while(p!=list && (p->data.sales) < (node->data.sales) && (p->data.play_id) < (node->data.play_id) )
		{
			p=p->next;
		}

		//将结点node加入到p之前
		List_InsertBefore(p, node);
	}
}


