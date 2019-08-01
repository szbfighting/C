#include "Sale_Persist.h"
#include "../Common/list.h"
#include "../Service/Sale.h"
#include <stdio.h>
#include<io.h>
#include<process.h>
#include <stdlib.h>
#include <string.h>


static const char SALE_DATA_FILE[] = "Sale.dat";
static const char SALE_DATA_TEMP_FILE[] = "SaleTmp.dat";

int Sale_Perst_Insert(const sale_t *data)
{
	assert(NULL!=data);
	FILE *fp = fopen(SALE_DATA_FILE, "ab");
	int rtn = 0;
	if (NULL == fp) {
		printf("Cannot open file %s!\n", SALE_DATA_FILE);
		return 0;
	}

	rtn = fwrite(data, sizeof(sale_t), 1, fp);
	
	fclose(fp);
	return rtn;

}


int Sale_Perst_DeleteByID(int saleID)
{

	sale_t buf;
	FILE *fd, *fp;

	fp = fopen(SALE_DATA_FILE, "rb");
	fd = fopen(SALE_DATA_TEMP_FILE, "wb");

	int found = 0;

	while ((fread(&buf, sizeof(sale_t), 1, fp))&&(!feof(fp)))
	{
		fwrite(&buf, sizeof(sale_t), 1, fd);
	}

	fclose(fd);
	fclose(fp);

	fp = fopen(SALE_DATA_FILE, "wb");
	fd = fopen(SALE_DATA_TEMP_FILE,"rb");

	while (!feof(fd))
	{

		if (fread(&buf, sizeof(sale_t), 1, fd))
		{
			if (saleID == buf.id)
			{
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(sale_t), 1, fp);
		}
	}



	fclose(fd);
	fclose(fp);

	remove(SALE_DATA_TEMP_FILE);
	return found;
	
}

//根据用户ID载入给定时间区间内的销售记录
int Sale_Perst_SelectByUsrID(sale_list_t list, int usrID, user_date_t stDate, user_date_t endDate)
{
	assert(NULL!=list);

//	List_Init(list, sale_node_t);
	FILE *fp = fopen(SALE_DATA_FILE, "rb");
	if (NULL == fp)
	{
		printf("%s打开失败！\n",SALE_DATA_FILE);
		return 0;
	}

	sale_t saleRec;
	sale_node_t* newNode;
	int recCount = 0;


	fread(&saleRec, sizeof(sale_t), 1, fp);
	while ((!feof(fp)))
	{
//printf("%d %d\n",usrID, saleRec.user_id);
//printf("time:%d %d\n",DateCmp(saleRec.date,stDate),DateCmp(saleRec.date,endDate));
			if ((usrID == saleRec.user_id) && DateCmp(saleRec.date,stDate)<=1 && DateCmp(saleRec.date,endDate)>=-1)
			{
				//printf("111\n");
				newNode = (sale_node_t*)malloc(sizeof(sale_node_t));
				newNode->data = saleRec;
				//printf("li:%d\n",newNode->data.value);
				List_AddTail(list, newNode);
				recCount++;
			}
			fread(&saleRec, sizeof(sale_t), 1, fp);
	}

		fclose(fp);

		return recCount;
	
}


//根据给定时间区间内的销售记录
int Sale_Perst_SelectByDate(sale_list_t list, user_date_t stDate, user_date_t endDate)
{

	FILE *fp = fopen(SALE_DATA_FILE, "rb");
	if (NULL == fp)
	{
		printf("%s打开失败！\n",SALE_DATA_FILE);
		return 0;
	}
	sale_t saleRec;
	sale_node_t* newNode;
	int recCount = 0;

	while (!feof(fp))
	{
		if (fread(&saleRec, sizeof(account_t), 1, fp))
		{
			if (DateCmp(saleRec.date,stDate)==1 && DateCmp(saleRec.date,endDate)==-1)
			{
				newNode = (sale_node_t*)malloc(sizeof(sale_node_t));
				newNode->data = saleRec;
				List_AddTail(list, newNode);
				recCount++;
			}

		}
	}

	fclose(fp);

	return recCount;
	

}





/*

//queryUid = null,query all the uid Sale information return the all uid Sale information number
//queryUid =  uid, query the uid Sale information return the uid Sale  number
 *
int  Sale_Infor_By_Uid(char * queryUid, Sale_list_t head)
{

	return 0;
}

int Sale_Infor_By_Time(char  queryTime[][30], Sale_list_t head)
{

	
	return 0;
}

//根据票的id获得票的基本信息 
int Select_Price_By_Playid(int id, ticket_t *buf)
{
	
		return 0;
}
*/

