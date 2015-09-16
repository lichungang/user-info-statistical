/*************************************************************************
	> File Name: Server.cpp
	> Author: LCG
	> Mail: lichungang521521.com
	> Created Time: Fri 07 Aug 2015 07:41:06 PM PDT
 ************************************************************************/
#include"Server.h"
#include"pool.h"
int main(int argc,char **argv)
{
	string host = "192.168.223.128";
	string user = "root";
	string pass = "mysql";
	string dbname = "Project";
	int poolSize = 10;

	Pool::initPool(host,user,pass,dbname,poolSize);

	ClientAcceptor Serv;

	return 0;
}
