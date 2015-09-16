/*************************************************************************
	> File Name: Client.h
	> Author: LCG
	> Mail: lichungang521521.com
	> Created Time: Fri 07 Aug 2015 02:18:37 AM PDT
 ************************************************************************/
#ifndef _CLIENT_H_
#define _CLIENT_H_

#include<unistd.h>
#include<sys/socket.h>
#include<fcntl.h>
#include<sys/types.h>
#include<poll.h>
#include<signal.h>
#include<arpa/inet.h>
//#include<netinet.h>
#include<fstream>
#include<cstring>
#include<iostream>

#define BINLOG		"../cfg/binlog.txt"
typedef enum Send_Type
{
	binlog = 1,
	t_msgnotify = 2,
	t_user_info = 3,
	t_bank_user = 4,
	t_relation = 5,
}Send_Type;

typedef enum DML_type
{
	Insert = 1,
	Update = 2,
	Delete = 3,
	Replace = 4,
}DML_type;

#define CLIENT_PORT		8000
#define CLIENT_ADDR		"192.168.223.128"
using namespace std;

class Client 
{
	public:
		Client();
		~Client();

		int SendTrueData();
		int SendFileData(char *buffer,int length);
		string Split(string str,Send_Type file,string buffer);
		int  ReadDataFromFile(int fileNo,Send_Type file);
		string FormData(DML_type,string buffer,Send_Type file);
		string ReadFile(DML_type d_type,string FileName,Send_Type file);
		int SendDataSize(DML_type d_type,Send_Type file);
		int SendFileData();

	private:
		string readData;
		int length;
		int SocketFd;
		struct sockaddr_in addr;
};

#endif	//_CLIENT_H_
