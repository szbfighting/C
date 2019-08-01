
#include "Account_Persist.h"
#include "../Service/account.h"
#include "../common/list.h"
#include <stdlib.h>
#include <stdio.h>
#include<io.h>
#include<process.h>
#include <assert.h>
#include <string.h>

static const char ACCOUNT_DATA_FILE[] = "Account.dat";
static const char ACCOUNT_DATA_TEMP_FILE[] = "AccountTmp.dat";

//判断账号文件是否存在，返回1存在，返回0不存在
int Account_Perst_CheckAccFile()
{
	 if ( fopen(ACCOUNT_DATA_FILE,"r")==NULL)
	 {
		 return 0;
	 }
	 else
	 {
		 return 1;
	 }
}

//根据用户名载入账号,载入成功 return 1；否则 return 0
int Account_Perst_SelByName(char usrName[], account_t *buf)
{

	FILE* fp;
	account_t account;

	if ((fp = fopen(ACCOUNT_DATA_FILE,"r")) == NULL)
	{
		printf("Account.dat文件打开失败！\n");
		return 0;
	}
	else
	{
		while(!feof(fp))
		{


			fread(&account,sizeof(account_t),1,fp);

			if (strcmp(usrName, account.username) == 0)
			{

				buf->id=account.id;
				strcpy(buf->password, account.password);
				buf->type=account.type;
				return 1;
			}
		}
		fclose(fp);
	}


	return 0;

}

//新账号写入账号文件中，返回实际写入的数据块数目
int Account_Perst_Insert(const account_t *data)
{
	assert(NULL!=data);
	FILE *fp = fopen(ACCOUNT_DATA_FILE, "ab");
	int rtn = 0;
	if (NULL == fp) {
		printf("Cannot open file %s!\n", ACCOUNT_DATA_FILE);
		return 0;
	}

	rtn = fwrite(data, sizeof(account_t), 1, fp);

	fclose(fp);
	return rtn;

}

//在账号文件中查找与参数账号匹配的账号，找到 return 1；否则 return 0；并进行覆盖重写
int Account_Perst_Update(const account_t * data) {

	assert(NULL!=data);

	FILE *fp = fopen(ACCOUNT_DATA_FILE, "rb+");
	if (NULL == fp) {
		printf("Cannot open file %s!\n", ACCOUNT_DATA_FILE);
		return 0;
	}

	account_t buf;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&buf, sizeof(account_t), 1, fp)) {
			if (buf.id == data->id) {
				fseek(fp, -sizeof(account_t), SEEK_CUR);
				fwrite(data, sizeof(account_t), 1, fp);
				found = 1;
				break;
			}

		}
	}
	fclose(fp);

	return found;

}

//在账号文件中删除与参数id匹配的账号，删除成功 return 1；否则 return 0；
int Account_Perst_DeleteByID(int id)
{
	account_t buf;
	FILE *fd, *fp;


	fp = fopen(ACCOUNT_DATA_FILE, "rb");
	fd = fopen(ACCOUNT_DATA_TEMP_FILE, "wb");

	int found = 0;

	while ((fread(&buf, sizeof(account_t), 1, fp))&&(!feof(fp)))
	{

		fwrite(&buf, sizeof(account_t), 1, fd);
	}

	fclose(fd);
	fclose(fp);

	fp = fopen(ACCOUNT_DATA_FILE, "wb");
	fd = fopen(ACCOUNT_DATA_TEMP_FILE,"rb");

	while (!feof(fd))
	{

		if (fread(&buf, sizeof(account_t), 1, fd))
		{
//			printf("%d %d\n",id,buf.id);
//			getchar();
			if (id == buf.id)
			{
				found = 1;
				continue;
			}
				fwrite(&buf, sizeof(account_t), 1, fp);

		}

	}



	fclose(fd);
	fclose(fp);

	remove(ACCOUNT_DATA_TEMP_FILE);
	return found;

}

//在账号文件中查找与参数id匹配的账号，并通过指针buf传出；匹配成功 return 1；否则 return 0；
int Account_Perst_SelectByID(int id, account_t *buf)
{

	assert(NULL!=buf);


	FILE *fp = fopen(ACCOUNT_DATA_FILE, "rb");
	if (NULL == fp)
	{
		return 0;
	}

	account_t data;
	int found = 0;

	while (!feof(fp))
	{
		if (fread(&data, sizeof(account_t), 1, fp))
		{
			if (id == data.id)
			{
				buf->id = id;
				strcpy(buf->password,data.password);
				strcpy(buf->username,data.username);
				buf->type = data.type;
//printf("%s %s %d",data.username,data.password,data.id);
				found = 1;
				break;
			}

		}
	}

	fclose(fp);

	return found;

}

//遍历读ACCOUNT_DATA_FILE文件，动态构建用户账号list链表,list 为链表头指针，返回list长度
int Account_Perst_SelectAll(account_list_t list)
{
	account_node_t *newNode;
	account_t data;
	int recCount = 0;

	assert(NULL!=list);

	List_Free(list, account_node_t);

	FILE *fp = fopen(ACCOUNT_DATA_FILE, "rb");
	if (NULL == fp)
	{
		return 0;
	}

	while (!feof(fp))
	{
		if (fread(&data, sizeof(account_t), 1, fp)) {
			newNode = (account_node_t*) malloc(sizeof(account_node_t));
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

