#include <stdio.h>
#include <stdlib.h>

#include "Queries_Menu.h"
#include "Schedule_UI.h"
#include "Studio_UI.h"
#include "Play_UI.h"
#include "Ticket_UI.h"
void Queries_Menu(void){
	char choice;
	do {
		system("cls");
		printf("\n");
		printf("\n");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t\t\t\t\t\t信息查询\n");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t\t\t\t[S]\t\t演出厅查询\n \n");
		printf("\t\t\t\t\t[P]\t\t剧目查询\n\n");
		printf("\t\t\t\t\t[T]\t\t票务查询\n\n");
		printf("\t\t\t\t\t[C]\t\t演出计划查询\n\n");
		printf("\t\t\t\t\t[R]\t\t返回上一级\n");
		printf("\n\t\t※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※\n");
		printf("\t\t请输入您想要办理的业务:");


		fflush(stdin);
		choice = getchar();
		//int id;
		switch (choice) {
		case 'S':
		case 's':
			Studio_UI_ListAll();
			break;
		case 'P':
		case 'p':
			Play_UI_MgtEntry(1);
			break;
		case 'T':
		case 't':
			ListTickets();
			break;
		case 'C':
		case 'c':
			Schedule_UI_ListAll();
			break;
		}
	} while ('R' != choice && 'r' != choice);
	}
