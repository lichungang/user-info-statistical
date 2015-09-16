/*************************************************************************
	> File Name: connectmysql.cpp
	> Author: LCG
	> Mail: lichungang521521.com
	> Created Time: Tue 04 Aug 2015 07:51:07 PM PDT
 ************************************************************************/
#ifndef _MYSQL_H_
#define _MYSQL_H_

#include<string>
#include<iostream>
#include<map>
#include<vector>
#include<string.h>
#include<stdio.h>
#include<cstring>
#include<sstream>
#include<mysql/mysql.h>
#include"pool.h"
using namespace std;


class connectMysql
{
	public:
	connectMysql(map<string,string> mapped_v);
	~connectMysql();
//	dealMysql GetConn();
	int Insert_t_relation(const string &DB_Name,const string &Fuid, const string &Fqqid,const string &sign);
	int Update_Column(const string &Table_Name,const string &Fuid,const string &Column,const string &value);
	int Update_Column_t_relation(const string &Fuid,const string &Fsign);
	int Insert_Column(const string &Table_Name,const string &Fuid,const string &Column,const string &value,const string &flag);
	int Update_User_info_backup(const string &Table_Name,const string &Fuid,const string &Column,const string &value);
	int Count_AddOrReduce_One(const string &Table_Name,const string &Column,const string &value,const string &flag);
	
	int Deal_Insert(const string &Table_Name,const string &Fuid,const string &Column,const string &value);
	int Delete_One_Record(const string &Table_Name,const string &Fuid);
	int Set_Column_NULL(const string &Fuid,const string &Column);
	int Deal_Delete(const string &Table_Name,const string &Fuid,const string &Column,const string &value);
	int Delete_t_relation(const string &Fqqid);

	int Chenge_Column(const string &Fmobile,const string &Fcreid,const string &Fbankid,const string &Fuid,const string &flag);
	int Deal_Update_t_relation(const string &Fqqid,const string &Fsign);
	int Deal_Update(const string &Table_Name,const string &Fuid,const string &Column,const string &value);

	int Deal_Replace(const string &Table_Name,const string &Fuid,const string &Column,const string &value);
	int Deal_Replace_t_relation(const string &Fuid,const string &Fqqid,const string &Fsign);

	int Replace(map<string,string> mapped_v);
	int Insert(map<string,string> mapped_v);
	int Delete(map<string,string> mapped_v);
	int Update(map<string,string> mapped_v);

	private:
		static Pool p;
		Connection *conn;
		//MYSQL *conn;
};

#endif		//_MYSQL_H_
