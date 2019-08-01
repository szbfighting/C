

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


static const char SCHEDULE_DATA_FILE[] = "Schedule.dat";//�����ݳ��ƻ���Ϣ
static const char SCHEDULE_DATA_TEMP_FILE[] = "ScheduleTmp.dat";////ɾ�������ʱ����ʱ�����ݳ��ƻ���Ϣ

/*
 * Function:    Schedule_Perst_Insert
 * Function ID:	TTMS_SCU_Schedule_Perst_Insert
 * Description: ���ݳ��ƻ���Ϣ�ļ�ĩβд��һ���ݳ��ƻ���Ϣ
 * Input:       �������ļ����ݳ��ƻ���Ϣ����
 * Output:      ���ļ�ʧ�ܵ���Ϣ
 * Return:      д���ļ��ļ�¼��
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
 * Description: �����ݳ��ƻ�ID�Ÿ����ļ��е��ݳ��ƻ���Ϣ
 * Input:       �����ļ��и��µ��ݳ��ƻ���Ϣ����
 * Output:      ���ļ�ʧ�ܵ���Ϣ
 * Return:      ���µ��ݳ��ƻ���Ϣ����0��ʾδ�ҵ���1��ʾ�ҵ�������
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
 * Description: �����ݳ��ƻ�ID��ɾ���ݳ��ƻ�����Ϣ
 * Input:       ��ɾ�����ݳ��ƻ�ID��
 * Output:      ���ļ�ʧ�ܵ���Ϣ
 * Return:      0��ʾɾ��ʧ�ܣ�1��ʾɾ���ɹ�
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
 * Description: �����ݳ��ƻ�ID��ɾ���ݳ��ƻ�����Ϣ
 * Input:       �����ҵ��ݳ��ƻ�ID�ţ�������ҽ�����ڴ�ĵ�ַ
 * Output:      ��
 * Return:      0��ʾδ�ҵ���1��ʾ�ҵ���
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
 * Description: �������ݳ��ƻ���Ϣ������һ������
 * Input:       listΪ�ݳ��ƻ���Ϣ�����ͷָ��
 * Output:      ��ʾ��������ʱ������ռ�ʧ��
 * Return:      ���ز��ҵ��ļ�¼��Ŀ
 */
int Schedule_Perst_SelectAll(schedule_list_t list){
	schedule_node_t *newNode;
		schedule_t data;
		int recCount = 0;

		assert(NULL!=list);

		//�ļ�������
		if (access(SCHEDULE_DATA_FILE, 0))
			return 0;

		List_Free(list, schedule_node_t);

		FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
		if (NULL == fp) { //�ļ�������
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
 * Description: ��ͬ���Ŀ���ݳ��ƻ���Ϣ��������������һ������
 * Input:       listΪ�����������ݳ��ƻ���Ϣ�����ͷָ�룬��Ŀ��ID��
 * Output:      ��ʾ��������ʱ������ռ�ʧ��
 * Return:      ���ز��ҵ��ļ�¼��Ŀ
 */
int Schedule_Perst_SelectByPlay(schedule_list_t list, int play_id){
	assert(NULL!=list);

	FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
	if (NULL == fp)
	{
		printf("%s��ʧ�ܣ�\n",SCHEDULE_DATA_FILE);
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
