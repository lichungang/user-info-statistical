#include <stdlib.h>
#include "pool.h"
using namespace std;
//using vector::std;
int poolSize_ = 5;

string Pool::host_ = "";
string Pool::user_ = "";
string Pool::pass_ = "";
string Pool::dbname_ = "";
pthread_mutex_t Pool::mutex;
Pool* Pool::pool_ = NULL;

void Pool::initlock()
{
	pthread_mutex_init(&mutex,0);
}

void Pool::lock()
{
	pthread_mutex_lock(&mutex);
}

void Pool::unlock()
{
	pthread_mutex_unlock(&mutex);
}

Pool::Pool(string host,string user,string pass,string dbname,int poolSize)
{
	initlock();
	array = new p_use[poolSize_];			//pool array establish

	for(int i=0; i<poolSize_; i++)
	{
		Connection* conn = new Connection(host,user,pass,dbname);
		if(!conn)
		{
			cout << " new Connection Operation failed" << endl;
			exit(-1);
		}
		array[i].conn = conn;				//add conn to array pool
		array[i].is_used = false;
	}
}

Pool::~Pool()
{
	for(int i=0;i<poolSize_;i++)
	{
		delete array[i].conn;
	}
	delete[] array;
	//mysql_library_end();
}

int Pool::initPool(const string host,const string user,const string pass,
								const string dbname,const int poolSize)
{
	host_ = host;
	user_ = user;
	pass_ = pass;
	dbname_ = dbname;
	poolSize_ = poolSize;
	
	return 0;
}

void Pool::destroyPool()
{
	if(pool_)
	{
		delete pool_;
		pool_ = NULL;
	}
}

Connection* Pool::getConnection()
{

	// init pool, open connections
	lock();
	if(pool_ == NULL)
	{

		//mysql_library_init(0,NULL,NULL);

		pool_ = new Pool(host_,user_,pass_,dbname_,poolSize_);
	}
	unlock();

	//get connection operation
	Connection* ret = NULL;
	while(true)
	{
		lock();
		bool flag = false;
		for(int i=0;i<poolSize_;i++)
		{
			if(pool_->array[i].is_used == false)
			{
				pool_->array[i].is_used = true;
				ret = pool_->array[i].conn;
				flag = true;
				break;
			}
		}
		if(flag == true)
		{
			unlock();
			break;
		}
		else
		{
			//cout << "wait" << endl;
			unlock();
			usleep(1000);
			continue;
		}
	}
	connFlag  = true;
	return ret;
}

int Pool::releaseConnection(Connection* conn)
{
	lock();
	for(int i=0;i<poolSize_;i++)
	{
		if(pool_->array[i].conn == conn )
		{
			pool_->array[i].is_used = false;
			break;
		}
	}
	unlock();
	return 1;
}


Connection::Connection(string host,string user,string pass,string dbname)
{
	conn = mysql_init(NULL);
	if(conn == NULL)
	{
		cout<<"Error init conn"<<mysql_error(conn)<<endl;
	}

	//set connection options
	if(mysql_options(conn,MYSQL_OPT_CONNECT_TIMEOUT,(char *)&Over_Time) != 0);
	{
		cout<<"set connection_timeout error"<<mysql_error(conn)<<endl;
	}

	//set read over_time
	if(mysql_options(conn,MYSQL_OPT_READ_TIMEOUT,(char *)&Read_Timeout) != 0)
	{
		cout<<"set read_timeout error"<<mysql_error(conn)<<endl;
	}

	//set write_timeout 
	if(mysql_options(conn,MYSQL_OPT_WRITE_TIMEOUT,(char *)&Write_Timeout) != 0)
	{
		cout<<"set write_timeout error"<<mysql_error(conn)<<endl;
	}

	cout <<"DB init ok!"<<endl;
	if (!mysql_real_connect(conn,"192.168.223.128", "root", "mysql","Project",3306,NULL,0))
	{
		printf( "Error connecting to database: %s\n",mysql_error(conn));
		exit(-1);
	}
}

Connection::~Connection()
{

	//mysql_thread_end();
	mysql_close(conn);
}

string Connection::Handle_Sql(const string &sql)
{
	int ret;
	int fieldNum;
	string result;

	ret = mysql_real_query(this->conn,sql.c_str(),sql.length());
	if(ret != 0)
	{
		cout << "error ?" << endl;
		mysql_close(this->conn);
		cout<<mysql_error(conn)<<endl;
	}

	cout<<"begin query..."<<endl;

	MYSQL_RES * rst = mysql_store_result(conn);
	if(rst != NULL)
	{
		fieldNum = mysql_num_fields(rst);
		MYSQL_ROW row = mysql_fetch_row(rst);

		if(row[0] != NULL)
		{
			result = row[0];
		}

		mysql_free_result(rst);
	}

	cout<<"result = "<<result<<endl;
	return result;
}
/*
string Connection::Select(const string &Table_Name,const string &Column,
								 const string &value,const string &flag)
{
	//stringstream ss;
	string result;
	string sql;
	stringstream ss;

	if(flag.compare("Insert") == 0)
	{
		ss<<"select "<<Column<<" from "<<Table_Name<<" where "<<Column
											<<"="<<value<<";"<<endl;
		sql = ss.str();
	}
	else if(flag.compare("Delete") == 0)
	{
		ss<<"select "<<Column<<" from "<<Table_Name<<" where Fuid="<<value
															<<";"<<endl;
		sql = ss.str();
	}
	else if(flag.compare("All") == 0)
	{
		ss<<"select count(*) from user_info_backup where Fmobile is null and "
						<<" Fcreid is null and Fbankid is null;"<<endl;
		sql = ss.str();
	}
	else if(flag.compare("DR") == 0)
	{
		ss<<"select "<<Column<<" from "<<Table_Name<<" where Fqqid="<<value
															<<";"<<endl;
		sql = ss.str();
	}
	else if(flag.compare("Count") == 0)
	{
		ss<<"select count(*) from "<<Table_Name<<" where "<<Column<<"="
													<<value<<";"<<endl;
		sql = ss.str();
	}

	cout<<"sql = "<<sql<<endl;
	result = Handle_Sql(sql);
	cout<<"result is:"<<result<<endl;

	return result;
}
*/
int Connection::Mysql_Query(const string &sql,const int &length)
{
	int ret;

	ret = mysql_real_query(conn,sql.c_str(),length);
	if(ret != 0)
	{
		cout<<"error "<<endl;
		mysql_close(conn);
		cout<<mysql_error(conn)<<endl;
	}

	return ret;
}


