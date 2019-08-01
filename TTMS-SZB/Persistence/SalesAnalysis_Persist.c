

#include "../Persistence/EntityKey_Persist.h"
//#include "../Service/play.h"
#include "../Service/Sale.h"
#include "../Service/salesanalysis.h"
#include "../Common/list.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

//static const char PLAY_DATA_FILE[] = "play.dat";
static const char SALE_DATA_FILE[] = "sale.dat";
static const char SALESANALYSIS_DATA_FILE[] = "salesanalysis.dat";

//��һ��salesanalysis��¼��*data��д��salesanalysis.dat�ļ����ɹ�return 1������return 0
int Salesanalysis_Perst_Insert(const salesanalysis_t *data) {

	assert(NULL!=data);
	FILE *fp = fopen(SALESANALYSIS_DATA_FILE, "ab");
	int rtn = 0;
	if (NULL == fp)
	{
		printf("Cannot open file %s!\n", SALESANALYSIS_DATA_FILE);
		return 0;
	}

	rtn = fwrite(data, sizeof(salesanalysis_t), 1, fp);

	fclose(fp);
	return rtn;

}

//������salesanalysis.dat�ļ��������۷�����salesanalysis������
int SalesAnalysis_Perst_SelectAll(salesanalysis_list_t list)
{
	salesanalysis_node_t *newNode;
	salesanalysis_t data;
	int recCount = 0;

	assert(NULL!=list);

	List_Free(list, salesanalysis_node_t);

	FILE *fp = fopen(SALESANALYSIS_DATA_FILE, "rb");
	if (NULL == fp)
	{
		return 0;
	}

	while (!feof(fp))
	{
		if (fread(&data, sizeof(salesanalysis_t), 1, fp)) {
			newNode = (salesanalysis_node_t*) malloc(sizeof(salesanalysis_node_t));
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

//������Sale.dat�ļ�����list��sale_list_t�� ��������������list����
int Sale_Perst_SelectAll(sale_list_t list)
{

	sale_node_t *newNode;
	sale_t data;
	int recCount = 0;

	assert(NULL!=list);

	List_Free(list, sale_node_t);

	FILE *fp = fopen(SALE_DATA_FILE, "rb");
	if (NULL == fp)
	{
		return 0;
	}

	while (!feof(fp))
	{
		if (fread(&data, sizeof(sale_t), 1, fp)) {
			newNode = (sale_node_t*) malloc(sizeof(sale_node_t));
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
