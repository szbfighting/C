
#include "Seat_Persist.h"
#include "../Service/Seat.h"
#include "../common/list.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <io.h>

static const char SEAT_DATA_FILE[] = "Seat.dat";
static const char SEAT_DATA_TEMP_FILE[] = "SeatTmp.dat";

int Seat_Perst_Insert(const seat_t *data) {
	assert(NULL != data);
	FILE *fp = fopen(SEAT_DATA_FILE, "ab");
	int rtn = 0;

	if (NULL == fp) {
		printf("Cannot open file %s!\n", SEAT_DATA_FILE);
		return 0;
	}

	rtn = fwrite(data, sizeof(seat_t), 1, fp);

	fclose(fp);
	return rtn;
}

int Seat_Perst_InsertBatch(seat_list_t list) {
	seat_node_t *p;
	assert(NULL != list);
	FILE *fp = fopen(SEAT_DATA_FILE, "ab");
	int rtn = 0;

	if (NULL == fp) {
		printf("Cannot open file %s!\n", SEAT_DATA_FILE);
		return 0;
	}

	List_ForEach(list, p) {
		fwrite(&(p->data), sizeof(seat_t), 1, fp);
		rtn++;
	}

	fclose(fp);
	return rtn;
}

int Seat_Perst_Update(const seat_t *seatdata) {
	assert(NULL != seatdata);

	FILE *fp = fopen(SEAT_DATA_FILE, "rb+");
	if (NULL == fp) {
		printf("Cannot open file %s!\n", SEAT_DATA_FILE);
		return 0;
	}

	seat_t buf;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&buf, sizeof(seat_t), 1, fp)) {
			if (buf.id == seatdata->id) {
				fseek(fp, -sizeof(seat_t), SEEK_CUR);
				fwrite(seatdata, sizeof(seat_t), 1, fp);
				found = 1;
				break;
			}
		}
	}
	fclose(fp);

	return found;
}

int Seat_Perst_DeleteByID(int ID) {
	//将原始文件重命名，然后读取数据重新写入到数据文件中，并将要删除的实体过滤掉。
	FILE *fpSour, *fpTarg;

	//对原始数据文件重命名
	if (rename(SEAT_DATA_FILE, SEAT_DATA_TEMP_FILE) < 0) {
		printf("Cannot open file %s!\n", SEAT_DATA_FILE);
		return 0;
	}

	fpSour = fopen(SEAT_DATA_TEMP_FILE, "rb");
	fpTarg = fopen(SEAT_DATA_FILE, "wb");

	if (NULL == fpTarg) {
		printf("Cannot open file %s!\n", SEAT_DATA_FILE);
		return 0;
	}
	if (NULL == fpSour) {
		printf("Cannot open file %s!\n", SEAT_DATA_TEMP_FILE);
		return 0;
	}

	seat_t buf;

	int found = 0;
	while (!feof(fpSour)) {
		if (fread(&buf, sizeof(seat_t), 1, fpSour)) {
			if (ID == buf.id) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(seat_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);

	//删除临时文件
	remove(SEAT_DATA_TEMP_FILE);

	return found;
}


int Seat_Perst_DeleteAllByRoomID(int roomID) {
	//将原始文件重命名，然后读取数据重新写入到数据文件中，并将要删除的实体过滤掉。
	FILE *fpSour, *fpTarg;

	//对原始数据文件重命名
	if (rename(SEAT_DATA_FILE, SEAT_DATA_TEMP_FILE) < 0) {
		printf("Cannot open file %s!\n", SEAT_DATA_FILE);
		return 0;
	}

	fpSour = fopen(SEAT_DATA_TEMP_FILE, "rb");
	fpTarg = fopen(SEAT_DATA_FILE, "wb");

	if (NULL == fpTarg) {
		printf("Cannot open file %s!\n", SEAT_DATA_FILE);
		return 0;
	}
	if (NULL == fpSour) {
		printf("Cannot open file %s!\n", SEAT_DATA_TEMP_FILE);
		return 0;
	}

	seat_t buf;

	int found = 0;
	while (!feof(fpSour)) {
		if (fread(&buf, sizeof(seat_t), 1, fpSour)) {
			if (roomID == buf.roomID) {
				found += 1;
				continue;
			}
			fwrite(&buf, sizeof(seat_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);

	//删除临时文件
	remove(SEAT_DATA_TEMP_FILE);

	return found;
}

int Seat_Perst_SelectByID(int ID, seat_t *buf) {
	assert(NULL != buf);

	FILE *fp = fopen(SEAT_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}

	seat_t data;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&data, sizeof(seat_t), 1, fp)) {
			if (ID == data.id) {
				*buf = data;
				found = 1;
				break;
			}
		}
	}
	fclose(fp);

	return found;
}

int Seat_Perst_SelectAll(seat_list_t list) {
	seat_node_t *newNode;
	seat_t data;
	int recCount = 0;

	assert(NULL != list);

	//文件不存在
	if (access(SEAT_DATA_FILE, 0))
		return 0;

	List_Free(list, seat_node_t);

	FILE *fp = fopen(SEAT_DATA_FILE, "rb");
	if (NULL == fp) //文件不存在
	{
		return 0;
	}

	while (!feof(fp)) {
		if (fread(&data, sizeof(seat_t), 1, fp)) {
			newNode = (seat_node_t*)malloc(sizeof(seat_node_t));
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

int Seat_Perst_SelectByRoomID(seat_list_t list, int roomID) {
	seat_node_t *newNode;
	seat_t data;
	int recCount = 0;

	assert(NULL != list);

	//文件不存在
	if (access(SEAT_DATA_FILE, 0)) {
		return 0;
	}

	List_Free(list, seat_node_t);

	FILE *fp = fopen(SEAT_DATA_FILE, "rb");
	if (NULL == fp) { //文件不存在
		return 0;
	}

	while (!feof(fp)) {
		if (fread(&data, sizeof(seat_t), 1, fp))
			if (data.roomID == roomID)  //若座位是本放映厅的座位，则读出
			{
				newNode = (seat_node_t*)malloc(sizeof(seat_node_t));
				if (!newNode) {
					printf(
						"Warning, Memory OverFlow!!!\n Cannot Load more Data into memmory!!!\n");
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