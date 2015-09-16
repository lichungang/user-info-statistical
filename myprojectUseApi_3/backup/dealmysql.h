/*************************************************************************
	> File Name: connectmysql.cpp
	> Author: LCG
	> Mail: lichungang521521.com
	> Created Time: Tue 04 Aug 2015 07:51:07 PM PDT
 ************************************************************************/
#ifndef _DEALMYSQL_H_
#define _DEALMYSQL_H_

#include<string>
#include<iostream>
#include<map>
#include<vector>
#include<string.h>
#include<stdio.h>
#include<cstring>
#include<sstream>
#include<mysql/mysql.h>
using namespace std;

class dealMysql
{
	public:
		int InitMysql(map<string,string> mapped_v);
		~dealMysql();
		MYSQL * GetConn();
		string Handle_Sql(const string &sql);
		string Select(const string &Table_Name,const string &Column,const string &value,const string &flag);
		int Mysql_Query(const string &sql,const int &length);
	private:
		MYSQL *retMysql;
		MYSQL mysql;
		MYSQL *conn;
};


#endif		//_DEALMYSQL_H_
