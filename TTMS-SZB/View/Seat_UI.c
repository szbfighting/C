#include "Seat_UI.h"

#include "../Service/Seat.h"
#include "../Service/Studio.h"
#include "../Service/EntityKey.h"
#include "../Common/list.h"

#include <stdio.h>

//根据状态返回对应表示状态符号
char Seat_UI_Status2Char(seat_status_t status) {

	char statusChar;
	switch (status) {
	case SEAT_GOOD:		//有座位
		statusChar = '#';
		break;
	case SEAT_BROKEN:	//损坏的座位
		statusChar = '~';
		break;
	case SEAT_NONE:
	default:
		statusChar = ' ';
		break;
	}
	return statusChar;
}

//根据状态符号返回桌位状态
seat_status_t Seat_UI_Char2Status(char statusChar) {
	seat_status_t status;
	switch (statusChar) {
	case '#':		//有座位
		status = SEAT_GOOD;
		break;
	case '~':	//损坏的座位
		status = SEAT_BROKEN;
		break;
	default:
		status = SEAT_NONE;
		break;
	}
	return status;
}

void Seat_UI_MgtEntry(int roomID) {
	int i, j;
	char choice;
	int seatCount;
	int changedCount = 0;
	studio_t studioRec;

	if (!Studio_Srv_FetchByID(roomID, &studioRec)) {  //获得对应id放映厅的信息
		printf("\t\t该放映厅不存在!\n按[Enter]键继续!\n");
		getchar();
		return;
	}

	seat_list_t list;
	seat_node_t *p;

	List_Init(list, seat_node_t);
	//选择放映厅的所有座位
	seatCount = Seat_Srv_FetchByRoomID(list, roomID);

	if (!seatCount) {		//若放映厅还没有设置座位，则自动生成座位
		seatCount = Seat_Srv_RoomInit(list, roomID, studioRec.rowsCount,
			studioRec.colsCount);
		//修改演出厅里的座位数量
		studioRec.seatsCount = seatCount;
		Studio_Srv_Modify(&studioRec);
	}
	do {

		//system("cls");
		printf("\n\t\t-----------------------------------------------------------------------------------\n");
		printf("\n\t\t\t\t\t %d号演出厅的座位列表\n",
			roomID);
		printf("\n\t\t-----------------------------------------------------------------------------\n");

		printf("%5c", ' ');
		printf(" "); 
		for (i = 1; i <= studioRec.colsCount; i++) {
			printf("%3d", i);
		}
		printf(
			"\n------------------------------------------------------\n");
		//显示数据
		for (i = 1; i <= studioRec.rowsCount; i++) {
			j = 1;
			printf("%2d行:%c", i, ' ');

			List_ForEach(list, p) {
				if (p->data.row == i) {
					while (p->data.column != j) {
						printf("%3c", ' ');
						j++;
					}
					printf("%3c", Seat_UI_Status2Char(p->data.status));
					j++;
				}
			}
			printf("\n");
		}

		printf("\n\t\t\t\t\[A]\t添加\t[D]\t删除\t[U]\t更新\t[R]\t返回\n");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'a':
		case 'A':
			changedCount = Seat_UI_Add(list, roomID, studioRec.rowsCount,
				studioRec.colsCount);
			if (changedCount > 0) {
				seatCount += changedCount;
				//修改演出厅里的座位数量
				studioRec.seatsCount = seatCount;
				Studio_Srv_Modify(&studioRec);
			}
			break;
		case 'd':
		case 'D':
			changedCount = Seat_UI_Delete(list, studioRec.rowsCount,
				studioRec.colsCount);
			if (changedCount > 0) {
				seatCount -= changedCount;
				//修改演出厅里的座位数量
				studioRec.seatsCount = seatCount;
				Studio_Srv_Modify(&studioRec);
			}
			break;
		case 'u':
		case 'U':
			Seat_UI_Modify(list, studioRec.rowsCount, studioRec.colsCount);
			break;
		}
	} while (choice != 'r' && choice != 'R');

	//释放链表空间
	List_Destroy(list, seat_node_t);
}

int Seat_UI_Add(seat_list_t list, int roomID, int row, int column) {  //输入一个座位
	seat_t rec;
	seat_node_t *p;
	int newRecCount = 0;
	char choice;
	do {
		system("cls");
		printf("\n\t--------------------------------------------------------------------------------------------\n");
		printf("\n\t\t\t\t添加新座位 \n");
		printf("\n\t--------------------------------------------------------------------------------------------\n");


		do {
			printf("\t\t输入的新的座位的行数和列数要比原来的大！");
			printf("\t\t座位的行数:");
			scanf("%d", &(rec.row));
			printf("\t\t座位的列数：");
			scanf("%d", &(rec.column));
			fflush(stdin);

		} while (rec.row > row || rec.column > column);

		p = Seat_Srv_FindByRowCol(list, rec.row, rec.column);
		if (p) {						//若输入的行列号所对应的座位已存在，则不能插入
			printf("座位已存在! \n");
			continue;
		}

		rec.id = EntKey_Srv_CompNewKey("Seat");		//设置座位id
		rec.roomID = roomID;
		rec.status = SEAT_GOOD;    //插入的新座位的状态默认为好座位
		printf(
			"==================================================================\n");

		if (Seat_Srv_Add(&rec)) {
			newRecCount++;
			printf("The new seat added successfully!\n");
			p = (seat_node_t*)malloc(sizeof(seat_node_t));
			p->data = rec;
			Seat_Srv_AddToSoftedList(list, p); //若增加了新座位，需更新list
		} else
			printf("The new seat added failed!\n");
		printf(
			"------------------------------------------------------------------\n");
		printf("[A]dd more, [R]eturn:");
		fflush(stdin);
		scanf("%c", &choice);
	} while ('a' == choice || 'A' == choice);
	return newRecCount;
}

int Seat_UI_Modify(seat_list_t list, int row, int column) {
	int rtn = 0;
	int newrow, newcolumn;
	char choice;
	seat_node_t *p;

	printf("\n\t\t--------------------------------------------------------------------------------------------\n");
	printf("\n\t\t\t\t\t座位状态更新 \n");
	printf("\n\t\t--------------------------------------------------------------------------------------------\n");

	do {
		do {				//更新的座位行列信息不能超出放映厅的行列数
							//printf("\t\t输入的新的座位的行数和列数要比原来的大！");
			printf("\t\t座位的行数:");
			scanf("%d", &newrow);
			printf("\t\t座位的列数：");
			scanf("%d", &newcolumn);
		} while (newrow > row || newcolumn > column);

		p = Seat_Srv_FindByRowCol(list, newrow, newcolumn);
		if (p) {
			printf("State of Seat [%d,%d]: [%c]:", newrow, newcolumn,
				Seat_UI_Status2Char(p->data.status));
			fflush(stdin);
			p->data.status = Seat_UI_Char2Status(getchar());
			printf(
				"-------------------------------------------------------------------\n");
			if (Seat_Srv_Modify(&(p->data))) {
				rtn = 1;
				printf("\t\t座位信息更新成功!\n");
			} else
				printf("\t\t座位信息更新失败!\n");

		} else
			printf("\t\t不存在这个座位！!\n");
		printf(
			"\t\t-------------------------------------------------------------------\n");
		printf("\t\t[U]\t更新\t\t[R]\t返回:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);
	} while ('u' == choice || 'U' == choice);
	return rtn;
}

int Seat_UI_Delete(seat_list_t list, int row, int column) {

	int delSeatCount = 0;
	int newrow, newcolumn;
	seat_node_t *p;
	char choice;

	do {
		system("cls");
		printf("\n\t\t--------------------------------------------------------------------------------------------\n");
		printf("\n\t\t\t\t\t删除座位 \n");
		printf("\n\t\t--------------------------------------------------------------------------------------------\n");

		do {
			fflush(stdin);

			printf("\t\t座位的行数:");
			scanf("%d", &(newrow));
			printf("\t\t座位的列数:");
			scanf("%d", &(newcolumn));
			fflush(stdin);
		} while (newrow > row || newcolumn > column);

		p = Seat_Srv_FindByRowCol(list, newrow, newcolumn);
		if (p) {
			printf(
				"\t\t==================================================================\n");
			if (Seat_Srv_DeleteByID(p->data.id)) {
				printf("\t\t座位删除成功啦!\n");

				delSeatCount++;
				List_FreeNode(p);	//释放结点座位结点p
			}
		} else {
			printf("\t\t座位删除失败了!\n");
		}

		printf(
			"\t\t------------------------------------------------------------------\n");
		printf("\t\t[D]\t继续删除\t\t[R]\t返回");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);
	} while ('d' == choice || 'D' == choice);
	return delSeatCount;
}
