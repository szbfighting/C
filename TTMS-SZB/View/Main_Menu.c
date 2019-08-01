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
		printf("\t\t\t\t\t       欢迎使用嘿嘿嘿票务管理系统  \n\n");
		printf("\t\t----------------------------------------------------------------------------------------\n\n");
		printf("\n");
		printf("\t\t\t\t\t[S]\t\t演出厅管理 \n");
		printf("\n");
		printf("\t\t\t\t\t[P]\t\t剧目管理\n");
		printf("\n");
		printf("\t\t\t\t\t[T]\t\t票务管理\n");
		printf("\n");
		printf("\t\t\t\t\t[R]\t\t退票管理\n");
		printf("\n");
		printf("\t\t\t\t\t[Q]\t\t查询\n");
		printf("\n");
		printf("\t\t\t\t\t[N]\t\t数据分析\n");
		printf("\n");
		printf("\t\t\t\t\t[A]\t\t用户管理 \n");
		printf("\n");
		printf("\t\t\t\t\t[E]\t\t退出\n");
		printf("\n");
		printf("\t\tжжжжжжжжжжжжжжжжжжжжжжжжжжжжжжжжжжжжжжжжжжжж\n");
		printf("\t\t你的选择：");
		
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

