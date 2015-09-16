#ifndef _POOL_H_
#define _POOL_H_
#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>
#include <map>
#include <vector>
#include <string>
#include<cstring>
#include<sstream>
#include <iostream>
#include <pthread.h>
#include<unistd.h>
using std::string;
using std::vector;

class Connection;

class p_use{
	public:
		Connection * conn;
		bool         is_used;
};

class Pool{
	public:
		Pool(string host,string user,string pass,string dbname,int poolSize);
		virtual ~Pool();
		static int initPool(const string host,const string user,const string pass,const string dbname,const int poolSize);
		static void destroyPool();

		Connection* getConnection();
		static int releaseConnection(Connection*);
		static void initlock();
		static void lock();
		static void unlock();

	private:

		static pthread_mutex_t mutex;
		static Pool* pool_;
		p_use* array;     // need an instance to init

	public:
		static string host_;
		static string user_;
		static string pass_;
		static string dbname_;
		bool	connFlag;		//connection state
		bool	isTrans;		//is in trans?
};

class Connection{
	public:
		Connection(string host,string user,string pass,string dbname);
		virtual ~Connection();

		string Handle_Sql(const string &sql);
		string Select(const string &Table_Name,const string &Column,const string &value,const string &flag);
		int Mysql_Query(const string &sql,const int &length);
//		void Rollback()
	public:
		// MYSQL代表了一条TCP连接
		MYSQL* conn;
	public:
		bool Over_Time;			//conntion over time
		bool Write_Timeout;		//
		bool Read_Timeout;		
};

#endif
