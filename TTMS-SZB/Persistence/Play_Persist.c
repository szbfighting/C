


#include "Play_Persist.h"
#include "../Service/play.h"
#include "../Common/list.h"
#include <stdlib.h>
#include <stdio.h>
#include<io.h>
#include<process.h>
#include <assert.h>
#include <string.h>

static const char PLAY_DATA_FILE[] = "Play.dat";//保存剧目信息
static const char PLAY_DATA_TEMP_FILE[] = "PlayTmp.dat";//删除或更新时，暂时保存剧目信息

/*
 * Function:    Play_Perst_Insert
 * Function ID:	TTMS_SCU_Play_Perst_Insert
 * Description: 在剧目信息文件末尾写入一条剧目信息
 * Input:       待加入文件的剧目信息数据
 * Output:      打开文件失败的信息
 * Return:      写入文件的记录数
 */
int Play_Perst_Insert(const play_t *data) {
	assert(NULL!=data);
	FILE *fp = fopen(PLAY_DATA_FILE, "ab");
	int rtn = 0;
	if (NULL == fp) {
		printf("Cannot open file %s!\n", PLAY_DATA_FILE);
		return 0;
	}

	rtn = fwrite(data, sizeof(play_t), 1, fp);

	fclose(fp);
	return rtn;
}

/*
 * Function:    Play_Perst_Update
 * Function ID:	TTMS_SCU_Play_Perst_Mod
 * Description: 按照剧目ID号更新文件中的剧目信息
 * Input:       待在文件中更新的剧目信息数据
 * Output:      打开文件失败的信息
 * Return:      更新的剧目信息数，0表示未找到，1表示找到并更新
 */
int Play_Perst_Update(const play_t *data) {
	assert(NULL!=data);

		FILE *fp = fopen(PLAY_DATA_FILE, "rb+");
		if (NULL == fp) {
			printf("Cannot open file %s!\n", PLAY_DATA_FILE);
			return 0;
		}

		play_t buf;
		int found = 0;

		while (!feof(fp)) {
			if (fread(&buf, sizeof(play_t), 1, fp)) {
				if (buf.id == data->id) {
					fseek(fp, -sizeof(play_t), SEEK_CUR);
					fwrite(data, sizeof(play_t), 1, fp);
					found = 1;
					break;
				}

			}
		}
		fclose(fp);

		return found;
}

/*
 * Function:    Play_Perst_DeleteByID
 * Function ID:	TTMS_SCU_Play_Perst_DelByID
 * Description: 按照剧目ID号删除剧目的信息
 * Input:       待删除的剧目ID号
 * Output:      打开文件失败的信息
 * Return:      0表示删除失败，1表示删除成功
 */
int Play_Perst_DeleteByID(int ID) {
	play_t buf;
	FILE *fd, *fp;


	fp = fopen(PLAY_DATA_FILE, "rb");
	fd = fopen(PLAY_DATA_TEMP_FILE, "wb");

	int found = 0;

	while ((fread(&buf, sizeof(play_t), 1, fp))&&(!feof(fp)))
	{

		fwrite(&buf, sizeof(play_t), 1, fd);
	}

	fclose(fd);
	fclose(fp);

	fp = fopen(PLAY_DATA_FILE, "wb");
	fd = fopen(PLAY_DATA_TEMP_FILE,"rb");

	while (!feof(fd))
	{

		if (fread(&buf, sizeof(play_t), 1, fd))
		{
//			printf("%d %d\n",id,buf.id);
//			getchar();
			if (ID == buf.id)
			{
				found = 1;
				continue;
			}
				fwrite(&buf, sizeof(play_t), 1, fp);

		}

	}



	fclose(fd);
	fclose(fp);

	remove(PLAY_DATA_TEMP_FILE);
	return found;

}

/*
 * Function:    Play_Perst_SelectByID
 * Function ID:	TTMS_SCU_Play_Perst_SelByID
 * Description: 按照剧目ID号查找剧目的信息
 * Input:       待查找的剧目ID号，保存查找结果的内存的地址
 * Output:      无
 * Return:      0表示未找到，1表示找到了
 */
int Play_Perst_SelectByID(int ID, play_t *buf) {
	assert(NULL!=buf);

	FILE *fp = fopen(PLAY_DATA_FILE, "rb");
	if (NULL == fp)
	{
		return 0;
	}

	play_t data;
	int found = 0;

	while (!feof(fp))
	{
		if (fread(&data, sizeof(play_t), 1, fp))
		{
			if (ID == data.id)
			{
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
 * Function:    Play_Perst_SelectAll
 * Function ID:	TTMS_SCU_Play_Perst_SelAll
 * Description: 将所有剧目信息建立成一条链表
 * Input:       list剧目信息链表的头指针
 * Output:      提示建立链表时，申请空间失败
 * Return:      返回查找到的记录数目
 */
int Play_Perst_SelectAll(play_list_t list) {
	play_node_t *newNode;
	play_t data;
	int recCount = 0;

	assert(NULL!=list);

	List_Free(list, play_node_t);

	FILE *fp = fopen(PLAY_DATA_FILE, "rb");
	if (NULL == fp)
	{
		return 0;
	}

	while (!feof(fp))
	{
		if (fread(&data, sizeof(play_t), 1, fp)) {
			newNode = (play_node_t*) malloc(sizeof(play_node_t));
			if (!newNode) {
				printf("cuocuocuo\n");
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
 * Function:    Play_Perst_SelectByName
 * Function ID:	TTMS_SCU_Play_Perst_SelByName
 * Description: 按照剧目名称查找剧目的信息
 * Input:       list为查找到的剧目信息链表，condt为模糊查询的关键字
 * Output:      提示建立链表时，申请空间失败
 * Return:      返回查找到的记录数目
 */
int Play_Perst_SelectByName(play_list_t list, char condt[]) {
	int recCount = 0;
	if ( access(PLAY_DATA_FILE ,0) != 0)
	{
		return 0;
	}
	List_Init(list,play_node_t);
	FILE* fp;
	play_t data;

	play_list_t newNode;
	if ((fp = fopen(PLAY_DATA_FILE,"r")) == NULL)
	{
		printf("Play.dat文件打开失败！\n");
		return 0;
	}

	while ((fread(&data, sizeof(play_t), 1, fp))&&(!feof(fp)))
	{
		if (strcmp(data.name,condt)==0) {

			newNode = (play_node_t*) malloc(sizeof(play_node_t));

			if (!newNode) {
				printf("cuocuocuo\n");
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

