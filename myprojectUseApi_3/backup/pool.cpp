#include <stdlib.h>
#include "pool.h"
using namespace std;

// static field init
string Pool::host_ = "";
string Pool::user_ = "";
string Pool::pass_ = "";
string Pool::dbname_ = "";
int    Pool::poolSize_ = 0;
Pool*  Pool::pool_ = NULL;
pthread_mutex_t Pool::mutex;
pthread_mutex_t Pool::execmutex;
pthread_mutex_t Pool::initmutex;
pthread_mutex_t Pool::logmutex;

void Pool::lock()
{
	pthread_mutex_lock(&mutex);
}

void Pool::unlock()
{
	pthread_mutex_unlock(&mutex);
}

void Pool::locke()
{
	pthread_mutex_lock(&execmutex);
}

void Pool::unlocke()
{
	pthread_mutex_unlock(&execmutex);
}

void Pool::locki()
{
	pthread_mutex_lock(&initmutex);
}

void Pool::unlocki()
{
	pthread_mutex_unlock(&initmutex);
}

void Pool::lockl()
{
	pthread_mutex_lock(&logmutex);
}

void Pool::unlockl()
{
	pthread_mutex_unlock(&logmutex);
}



Pool::Pool(string host,string user,string pass,string dbname,int poolSize)
{
	vec = new some[Pool::poolSize_];

	for(int i=0; i<poolSize_; i++)
	{
		Connection* conn = new Connection(Pool::host_,Pool::user_,Pool::pass_,Pool::dbname_);
		if(!conn)
		{
			cout << "xPool: new Connection Operation failed" << endl;
			exit(-1);
		}
		vec[i].first = conn;
		vec[i].second = false;
	}
}

Pool::~Pool()
{
	for(int i=0;i<poolSize_;i++)
	{
		delete vec[i].first;
	}
	delete[] vec;
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
	Pool::locki();
	if(pool_ == NULL)
	{
		pthread_mutex_init(&mutex,0);
		pthread_mutex_init(&execmutex,0);

		//mysql_library_init(0,NULL,NULL);

		pool_ = new Pool(host_,user_,pass_,dbname_,poolSize_);
	}
	Pool::unlocki();

	//get connection operation
	Connection* ret = NULL;
	while(true)
	{
		Pool::lock();
		bool flag = false;
		for(int i=0;i<poolSize_;i++)
		{
			if(pool_->vec[i].second == false)
			{
				pool_->vec[i].second = true;
				ret = pool_->vec[i].first;
				flag = true;
				break;
			}
		}
		if(flag == true)
		{
			Pool::unlock();
			break;
		}
		else
		{
			//cout << "wait" << endl;
			Pool::unlock();
			usleep(1000);
			continue;
		}
	}
	return ret;
}

int Pool::releaseConnection(Connection* conn)
{
	lock();
	for(int i=0;i<poolSize_;i++)
	{
		if(pool_->vec[i].first == conn )
		{
			pool_->vec[i].second = false;
			break;
		}
	}
	unlock();
	return 1;
}


Connection::Connection(string host,string user,string pass,string dbname)
{

	static int connectionCount = 0;

	conn = mysql_init(NULL);

	if (!mysql_real_connect(conn,"127.0.0.1", "root", "",dbname.c_str(),3306,NULL,0))
	{
		printf( "xPool: Error connecting to database: %s\n",mysql_error(conn));
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


