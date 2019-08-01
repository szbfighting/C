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
		printf("\t\t\t\t\t\t\t��Ϣ��ѯ\n");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t\t\t\t[S]\t\t�ݳ�����ѯ\n \n");
		printf("\t\t\t\t\t[P]\t\t��Ŀ��ѯ\n\n");
		printf("\t\t\t\t\t[T]\t\tƱ���ѯ\n\n");
		printf("\t\t\t\t\t[C]\t\t�ݳ��ƻ���ѯ\n\n");
		printf("\t\t\t\t\t[R]\t\t������һ��\n");
		printf("\n\t\t��������������������������������������������������������������������������������������\n");
		printf("\t\t����������Ҫ�����ҵ��:");


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
