
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

using std::string;
using std::vector;

class Connection;

class some{
	public:
		some(){}
		Connection * first;
		bool second;
};

class Pool{
	public:
		static int initPool(const string host,const string user,const string pass,const string dbname,const int poolSize);
		static void destroyPool();

		static Connection* getConnection();
		static int releaseConnection(Connection*);
		static void lock();
		static void unlock();

		static void locke();
		static void unlocke();

		static void locki();
		static void unlocki();

		static void lockl();
		static void unlockl();


	private:

		static pthread_mutex_t mutex;
		static pthread_mutex_t execmutex;
		static pthread_mutex_t initmutex;
		static pthread_mutex_t logmutex;
		static Pool* pool_;

	private:
		some* vec;     // need an instance to init

	public:
		static string host_ ;
		static string user_ ;
		static string pass_ ;
		static string dbname_;
		static int poolSize_ ;

	public:

		Pool(string host,string user,string pass,string dbname,int poolSize);
		virtual ~Pool();
};

class QueryResult;

class Connection{
	public:
		Connection(string host,string user,string pass,string dbname);
		virtual ~Connection();

		string Handle_Sql(const string &sql);
		string Select(const string &Table_Name,const string &Column,const string &value,const string &flag);
		int Mysql_Query(const string &sql,const int &length);
	//	QueryResult executeQuery(string statement);
	public:
		// MYSQL代表了一条TCP连接
		MYSQL* conn;
};

class QueryResult{
	public:
		int getRowCount(){return string_table.size();}
		int getColumnCount(){return string_table[0].size();};
		string getElement(int row,int column){return string_table[row][column];}
		void addRow(const vector<string>& row){string_table.push_back(row);}
	private:
		vector<vector<string> > string_table;
};
