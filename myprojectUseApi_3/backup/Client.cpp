/*************************************************************************
	> File Name: Clent.cpp
	> Author: LCG
	> Mail: lichungang521521.com
	> Created Time: Fri 07 Aug 2015 02:31:28 AM PDT
 ************************************************************************/
#include"Client.h"

int main(int argc,char **argv)
{
	
	Client client;

	DML_type d_type = Insert;
//	DML_type d_type = Delete;
//	DML_type d_type = Update;
//	DML_type d_type = Replace;
//	Send_Type s_type = t_user_info;
//	Send_Type s_type = t_bank_user;
	Send_Type s_type = t_msgnotify;
//	Send_Type s_type = t_relation;
	client.SendDataSize(d_type,s_type);

	client.SendTrueData();
	/*after connected,we need send source data*/
	cout << "....." << endl;


	return 0;
}

