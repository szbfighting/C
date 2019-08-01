

#include "Schedule_Persist.h"
#include "../Service/schedule.h"
#include "../Common/list.h"
#include "../Service/ticket.h"
//#include "../Persistence/Common_Persist.h"
#include "../View/Ticket_UI.h"
#include <stdlib.h>
#include <stdio.h>
#include<io.h>
#include<process.h>
#include <assert.h>


static const char SCHEDULE_DATA_FILE[] = "Schedule.dat";//保存演出计划信息
static const char SCHEDULE_DATA_TEMP_FILE[] = "ScheduleTmp.dat";////删除或更新时，暂时保存演出计划信息

/*
 * Function:    Schedule_Perst_Insert
 * Function ID:	TTMS_SCU_Schedule_Perst_Insert
 * Description: 在演出计划信息文件末尾写入一条演出计划信息
 * Input:       待加入文件的演出计划信息数据
 * Output:      打开文件失败的信息
 * Return:      写入文件的记录数
 */
int Schedule_Perst_Insert(const schedule_t *data)
{
	assert(NULL!=data);
	FILE *fp = fopen(SCHEDULE_DATA_FILE, "ab");
	int rtn = 0;
	if (NULL == fp) {
		printf("Cannot open file %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	rtn = fwrite(data, sizeof(schedule_t), 1, fp);

	fclose(fp);
	return rtn;

}

/*
 * Function:    Schedule_Perst_Update
 * Function ID:	TTMS_SCU_Schedule_Perst_Mod
 * Description: 按照演出计划ID号更新文件中的演出计划信息
 * Input:       待在文件中更新的演出计划信息数据
 * Output:      打开文件失败的信息
 * Return:      更新的演出计划信息数，0表示未找到，1表示找到并更新
 */
int Schedule_Perst_Update(const schedule_t *data){
	assert(NULL!=data);

	FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb+");
	if (NULL == fp) {
		printf("Cannot open file %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	schedule_t buf;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&buf, sizeof(schedule_t), 1, fp)) {
			if (buf.id == data->id) {
				fseek(fp, -sizeof(schedule_t), SEEK_CUR);
				fwrite(data, sizeof(schedule_t), 1, fp);
				found = 1;
				break;
			}

		}
	}
	fclose(fp);

	return found;

}

/*
 * Function:    Schedule_Perst_DeleteByID
 * Function ID:	TTMS_SCU_Schedule_Perst_DelByID
 * Description: 按照演出计划ID号删除演出计划的信息
 * Input:       待删除的演出计划ID号
 * Output:      打开文件失败的信息
 * Return:      0表示删除失败，1表示删除成功
 */
int Schedule_Perst_DeleteByID(int ID){
	schedule_t buf;
	FILE *fd, *fp;


	fp = fopen(SCHEDULE_DATA_FILE, "rb");
	fd = fopen(SCHEDULE_DATA_TEMP_FILE, "wb");

	int found = 0;

	while ((fread(&buf, sizeof(schedule_t), 1, fp))&&(!feof(fp)))
	{

		fwrite(&buf, sizeof(schedule_t), 1, fd);
	}

	fclose(fd);
	fclose(fp);

	fp = fopen(SCHEDULE_DATA_FILE, "wb");
	fd = fopen(SCHEDULE_DATA_TEMP_FILE,"rb");

	while (!feof(fd))
	{

		if (fread(&buf, sizeof(schedule_t), 1, fd))
		{
//			printf("%d %d\n",id,buf.id);
//			getchar();
			if (ID == buf.id)
			{
				found = 1;
				continue;
			}
				fwrite(&buf, sizeof(schedule_t), 1, fp);

		}

	}



	fclose(fd);
	fclose(fp);

	remove(SCHEDULE_DATA_TEMP_FILE);
	return found;

}

/*
 * Function:    Schedule_Perst_SelectByID
 * Function ID:	TTMS_SCU_Schedule_Perst_SelByID
 * Description: 按照演出计划ID号删除演出计划的信息
 * Input:       待查找的演出计划ID号，保存查找结果的内存的地址
 * Output:      无
 * Return:      0表示未找到，1表示找到了
 */
int Schedule_Perst_SelectByID(int ID, schedule_t *buf){
	assert(NULL!=buf);

	FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}

	schedule_t data;
	int found = 0;

	while (!feof(fp)){
		if (fread(&data, sizeof(schedule_t), 1, fp)){
			if (ID == data.id){
				*buf = data;
				found = 1;
				break;
			}
		}
	}
	fclose(fp);

	return found;

}

/*
 * Function:    Schedule_Perst_SelectAll
 * Function ID:	TTMS_SCU_Schedule_Perst_SelAll
 * Description: 将所有演出计划信息建立成一条链表
 * Input:       list为演出计划信息链表的头指针
 * Output:      提示建立链表时，申请空间失败
 * Return:      返回查找到的记录数目
 */
int Schedule_Perst_SelectAll(schedule_list_t list){
	schedule_node_t *newNode;
		schedule_t data;
		int recCount = 0;

		assert(NULL!=list);

		//文件不存在
		if (access(SCHEDULE_DATA_FILE, 0))
			return 0;

		List_Free(list, schedule_node_t);

		FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
		if (NULL == fp) { //文件不存在
			printf("Cannot open file %s!\n", SCHEDULE_DATA_FILE);
			return 0;
		}

		while (!feof(fp)) {
			if (fread(&data, sizeof(schedule_t), 1, fp)){
				newNode = (schedule_node_t*) malloc(sizeof(schedule_node_t));
				if (!newNode) {
					printf(
							"Warning, Memory OverFlow!!!\n Cannot Load more Data into memory!!!\n");
					break;
				}
				newNode->data = data;
				List_AddTail(list, newNode);
				recCount++;
			}
		}
		fclose(fp);
		return recCount;

}

/*
 * Function:    Schedule_Perst_SelectByPlay
 * Function ID:	TTMS_SCU_Schedule_Perst_SelByPlay
 * Description: 将同意剧目的演出计划信息搜索出来，建立一条链表
 * Input:       list为满足条件的演出计划信息链表的头指针，剧目的ID号
 * Output:      提示建立链表时，申请空间失败
 * Return:      返回查找到的记录数目
 */
int Schedule_Perst_SelectByPlay(schedule_list_t list, int play_id){
	assert(NULL!=list);

	FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
	if (NULL == fp)
	{
		printf("%s打开失败！\n",SCHEDULE_DATA_FILE);
		return 0;
	}

	schedule_t scheduleRec;
	schedule_node_t* newNode;
	int recCount = 0;

	while (!feof(fp))
	{
		if (fread(&scheduleRec, sizeof(schedule_t), 1, fp))
		{
			if (play_id == scheduleRec.play_id)
			{
				newNode = (schedule_node_t*)malloc(sizeof(schedule_node_t));
				newNode->data = scheduleRec;
				List_AddTail(list, newNode);
				recCount++;
			}

			}
		}

		fclose(fp);

		return recCount;


}
