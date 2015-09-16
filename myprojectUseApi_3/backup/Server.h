/*************************************************************************
	> File Name: Client.h
	> Author: LCG
	> Mail: lichungang521521.com
	> Created Time: Fri 07 Aug 2015 02:18:37 AM PDT
 ************************************************************************/
#ifndef _SERVER_H_
#define _SERVER_H_

#include<sys/socket.h>
#include<fcntl.h>
#include<signal.h>
#include<assert.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<string>
#include<fstream>
#include<map>
#include<cstring>
#include<vector>
#include<iostream>
#include"Client.h"
/*typedef enum send_Type
{
	binlog = 1,
	t_msgnotify = 2,
	t_user_info = 3,
	t_bank_user = 4,
	t_relation = 5,
}send_Type;
*/
#define SERVER_PORT		8000
#define MAX_EVENT_NUMBER 1024

using namespace std;

struct fds
{
		int sockfd;
		int ClientFd;
		int epollfd;
};

int RecvData(const int &connfd);
void * worker(void *arg);

class ClientAcceptor 
{
	public:
		
		ClientAcceptor();
		~ClientAcceptor();
		int Epoll_Register(const int &epollfd,epoll_event * events);
		void Addfd(const int &epollfd,const int &fd,const bool &flag);
		void reset_oneshot(const int &epollfd,const int &fd);
	private:
		char buf[1000];
		struct sockaddr_in addr;
		int SocketFd;
};

class DataProcessHelper
{
	public:
	/*	int Insert();
		int Update();
		int Delete();
		int Replace();
	*/	
		int CallHandleFunction(map<string,string> mapped_v);
		int  Initial(const string & data);
		
	private:
		vector<string> DB_Name;
		vector<string> Table_Name;
		vector<DML_type> DML_Type;
		vector<Send_Type> Send_type;
		vector<string> m_FieldValueVector;
		string m_FieldHead;

};
#endif	//_CLIENT_H_
