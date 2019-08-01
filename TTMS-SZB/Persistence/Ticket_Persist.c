#include "Ticket_Persist.h"
//#include "../Persistence/Common_Persist.h"
#include "../Service/ticket.h"
#include "../Service/seat.h"
#include "../common/list.h"
#include "Schedule_Persist.h"
#include "Play_Persist.h"
#include "EntityKey_Persist.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <io.h>

static const char TICKET_DATA_FILE[]="Ticket.dat";
static const char TICKET_DATA_TEMP_FILE[]="TicketTemp.dat";

int Ticket_Perst_Insert(ticket_list_t list){
	ticket_node_t *pos;
	ticket_t data;

	int rtn = 0;

	FILE *fp = fopen(TICKET_DATA_FILE, "ab");
	if (NULL == fp) {
		printf("Cannot open file %s!\n", TICKET_DATA_FILE);
		return 0;
	}
	schedule_t sch;
	play_t pla;
	Schedule_Perst_SelectByID(list->next->data.schedule_id, &sch);
	Play_Perst_SelectByID(sch.play_id, &pla);
	int count = 0;
	List_ForEach(list, pos) {
		count++;
	}
	int key = EntKey_Perst_GetNewKeys("ticket", count);

	List_ForEach(list, pos) {
		data.id = key++;
		data.price = pla.price;
		data.schedule_id = sch.id;
		data.seat_id = pos->data.seat_id;
		data.status = TICKET_AVL;
		fwrite(&data, sizeof(ticket_t), 1, fp);
	}
	fclose(fp);
	return rtn;
}
int Ticket_Perst_Delete(int schedule_id){

	char cmd[100];
	sprintf(cmd, "rename  %s %s", TICKET_DATA_FILE, TICKET_DATA_TEMP_FILE);
	system(cmd);

	FILE *fpSour, *fpTarg;
	fpSour = fopen(TICKET_DATA_TEMP_FILE, "rb");
	fpTarg = fopen(TICKET_DATA_FILE, "wb");
	if (NULL == fpTarg || NULL == fpSour) {
		printf("Cannot open file %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	ticket_t buf;

	int found = 0;
	while (!feof(fpSour)) {
		if (fread(&buf, sizeof(ticket_t), 1, fpSour)) {
			if (schedule_id == buf.schedule_id) {
				found ++;
				continue;
			}
			fwrite(&buf, sizeof(ticket_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);

	remove(TICKET_DATA_TEMP_FILE);

	return found;

}

int Ticket_Perst_Update(const ticket_t * data) {
	assert(NULL!=data);

	FILE *fp = fopen(TICKET_DATA_FILE, "rb+");
	if (NULL == fp) {
		printf("Cannot open file %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	ticket_t buf;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&buf, sizeof(ticket_t), 1, fp)) {
			if (buf.id == data->id) {
				fseek(fp, -sizeof(ticket_t), SEEK_CUR);
				fwrite(data, sizeof(ticket_t), 1, fp);
				found = 1;
				break;
			}

		}
	}
	fclose(fp);

	return found;
}


int Ticket_Perst_SelectByID(int ID, ticket_t *buf) {
	assert(NULL!=buf);

	FILE *fp = fopen(TICKET_DATA_FILE, "rb");
	if (NULL == fp) {
		printf("Cannot open file %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	ticket_t data;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&data, sizeof(ticket_t), 1, fp)) {
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

int Ticket_Perst_SelectAll(ticket_list_t list) {
	ticket_node_t *newNode;
	ticket_t data;
	int recCount = 0;

	assert(NULL!=list);

	//�ļ�������
	if (access(TICKET_DATA_FILE, 0))
		return 0;

	List_Free(list, ticket_node_t);

	FILE *fp = fopen(TICKET_DATA_FILE, "rb");
	if (NULL == fp) { //�ļ�������
		printf("Cannot open file %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	while (!feof(fp)) {
		if (fread(&data, sizeof(ticket_t), 1, fp)){
			newNode = (ticket_node_t*) malloc(sizeof(ticket_node_t));
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


int Ticket_Perst_SelectBySchID(ticket_list_t list, int schedule_id) {
	ticket_node_t *newNode;
	ticket_t data;
	int recCount = 0;

	assert(NULL!=list);

	//�ļ�������
	if (access(TICKET_DATA_FILE, 0))
		return 0;

	List_Free(list, ticket_node_t);

	FILE *fp = fopen(TICKET_DATA_FILE, "rb");
	if (NULL == fp) { //�ļ�������
		printf("Cannot open file %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	while (!feof(fp)) {
		if (fread(&data, sizeof(ticket_t), 1, fp)&&data.schedule_id==schedule_id){
			newNode = (ticket_node_t*) malloc(sizeof(ticket_node_t));
			if (!newNode) {
				printf("Warning, Memory OverFlow!!!\n Cannot Load more Data into memory!!!\n");
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

//����Ʊ�����б�����Ʊ
int Ticket_Perst_SelectByKeyList(ticket_list_t list, entkey_list_t keyList) {
	ticket_node_t *newNode;
	ticket_t data;
	int recCount = 0;

	assert(NULL!=list && keyList);


	List_Free(list, ticket_node_t);

	FILE *fp = fopen(TICKET_DATA_FILE, "rb");
	if (NULL == fp) { //�ļ�������
		printf("Cannot open file %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	while (!feof(fp)) {
		if (fread(&data, sizeof(ticket_t), 1, fp)
				&& EntKey_Srv_CheckExist(keyList, data.id)) {
			newNode = (ticket_node_t*) malloc(sizeof(ticket_node_t));
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
