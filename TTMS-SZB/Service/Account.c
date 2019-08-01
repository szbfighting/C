
#include "Account.h"
#include "EntityKey.h"
#include "../Common/list.h"
#include "../Persistence/Account_Persist.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern account_t gl_CurUser;

//����ϵͳ��ʼ���˺�admin
void Account_Srv_InitSys()
{

	if (Account_Perst_CheckAccFile() == 1)
	{
		return ;
	}

	account_t data_admin = {EntKey_Srv_CompNewKeys("account",1), USR_ANOMY, "admin", "admin_password"};

	data_admin.id = EntKey_Srv_CompNewKeys("account",1);

	strcpy(data_admin.username, "admin");
	strcpy(data_admin.password, "admin_password");
	data_admin.type = USR_ANOMY;

	if (Account_Srv_Add(&data_admin) == 0)
	{
		printf("Account_Srv_Add()��ʼ��ʧ�ܣ�\n");
	}

	return ;
}

//��֤��¼�˺��Ƿ��Ѵ��ڣ����ڣ������¼�û���Ϣ��ȫ�ֱ���gl_CurUser��return 1������return 0
int Account_Srv_Verify(char usrName[], char pwd[])
{
	account_t usr;

	strcpy(usr.username, usrName);
	strcpy(usr.password, pwd);


	if (Account_Perst_SelByName(usrName, &usr) == 1)
	{

		if(strcmp(pwd, usr.password) == 0)
		{

			gl_CurUser=usr;
			gl_CurUser.type = usr.type;
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}

	return 1;
}


//��֤�û����Ƿ���ڣ�����list�������ڣ����ض�Ӧ���ָ�룻���򣬷���null
account_node_t * Account_Srv_FindByUsrName(account_list_t list, char usrName[]) {

	assert(NULL!=list);
	account_node_t *ptr=list->next;

	while(ptr!=list){

		if( strcmp(ptr->data.username, usrName) == 0 )
		{
			return ptr;

		}
		else
		{
			ptr=ptr->next;
		}
	}

	return NULL;
}

//���һ���û��˺ţ�ͨ������Account_Perst_Insert(data)����ʵ��
int Account_Srv_Add(const account_t *data)
{
	return Account_Perst_Insert(data);

}

//�޸�һ���û��˺ţ�ͨ������Account_Perst_Update(data)����ʵ��
int Account_Srv_Modify(const account_t *data){

	return Account_Perst_Update(data);
}

//ɾ��һ���û��˺ţ�ͨ������Account_Perst_DeleteByID(usrID)����ʵ��
int Account_Srv_DeleteByID(int usrID)
{
	return Account_Perst_DeleteByID(usrID);
}

//��ȡusrID��Ӧ���û��˺���Ϣ��ͨ������Account_Perst_SelectByID(usrID, buf)����ʵ��
int Account_Srv_FetchByID(int usrID, account_t *buf)
{
	Account_Perst_SelectByID(usrID, buf);
	return 1;
}

//��ȡusrName��Ӧ���û��˺���Ϣ��ͨ������Account_Perst_SelByName(usrName, buf)����ʵ��
int Account_Srv_FetchByName(char usrName[], account_t *buf)
{

	return Account_Perst_SelByName(usrName, buf);

}

//��ȡ�����û��˺���Ϣ�����浽list�����У�ͨ������Account_Perst_SelectAll(list)����ʵ��
int Account_Srv_FetchAll(account_list_t list)
{
	return Account_Perst_SelectAll(list);
}

