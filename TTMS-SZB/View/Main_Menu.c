#include <stdio.h>

#include "Main_Menu.h"
#include "Schedule_UI.h"
#include "Studio_UI.h"
#include "Play_UI.h"
#include "Queries_Menu.h"
#include "Account_UI.h"
#include "Sale_UI.h"
#include "SalesAnalysis_UI.h"


#include<windows.h>
#include<conio.h>
void Main_Menu(void) {
	char choice;
	do {
		system("cls");
		printf("\n\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\t\t\t\t\t       ��ӭʹ�úٺٺ�Ʊ�����ϵͳ  \n\n");
		printf("\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\n");
		printf("\t\t\t\t\t[S]\t\t�ݳ������� \n");
		printf("\n");
		printf("\t\t\t\t\t[P]\t\t��Ŀ����\n");
		printf("\n");
		printf("\t\t\t\t\t[T]\t\tƱ�����\n");
		printf("\n");
		printf("\t\t\t\t\t[R]\t\t��Ʊ����\n");
		printf("\n");
		printf("\t\t\t\t\t[Q]\t\t��ѯ\n");
		printf("\n");
		printf("\t\t\t\t\t[N]\t\t���ݷ���\n");
		printf("\n");
		printf("\t\t\t\t\t[A]\t\t�û����� \n");
		printf("\n");
		printf("\t\t\t\t\t[E]\t\t�˳�\n");
		printf("\n");
		printf("\t\t�ااااااااااااااااااااااااااااااااااااااااااا�\n");
		printf("\t\t���ѡ��");
		
		fflush(stdin);

		choice = getchar();
		
		switch (choice) {
		case 'S':
		case 's':
			Studio_UI_MgtEntry();
			break;
		case 'P':
		case 'p':
			Play_UI_MgtEntry(0);
			break;
		case 'Q':
		case 'q':
			Queries_Menu();
			break;
		case 'T':
		case 't':
			Sale_UI_MgtEntry();
			break;
		case 'R':
		case 'r':
			Sale_UI_ReturnTicket();
			break;
		case 'N':
		case 'n':
			SalesAanalysis_UI_MgtEntry();
			break;
		case 'A':
		case 'a':
			Account_UI_MgtEntry();
			break;
		}
	} while ('E' != choice && 'e' != choice);
}

