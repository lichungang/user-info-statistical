/*************************************************************************
    > File Name: connectmysql.cpp
    > Author: LCG
    > Mail: lichungang521521.com
    > Created Time: Tue 04 Aug 2015 07:51:07 PM PDT
 ************************************************************************/
#include"connectmysql.h"
//#include"dealmysql.h"
connectMysql::connectMysql(map<string,string> mapped_v)
{
	conn = p.getConnection();
//	conn = Pool::getConnection();

	if(!conn)
	{
		cout<<"getConnection NULL pointer"<<endl;
	}
}

connectMysql::~connectMysql()
{
	Pool::releaseConnection(conn);
}

int connectMysql::Count_AddOrReduce_One(const string &Table_Name,const string &Column,
											const string &value,const string &flag)
{
	string sql;
	stringstream ss;

    if(flag.compare("Add") == 0)
    {
	   ss << "update "<<Table_Name<<" set Count=Count+1 where "<<Column<<"="
															<<value<<";" <<endl;
	   sql = ss.str();
    }
    else if(flag.compare("Reduce") == 0)
    {
		ss <<"update "<< Table_Name <<" set Count=Count-1 where "<<Column
														<<"="<<value<<";"<<endl;
	   sql = ss.str();
    }

    cout << "sql =" << sql << endl;

	int ret = conn->Mysql_Query(sql,sql.length());
    if(ret == 0)
    {
       if(flag.compare("Add") == 0)
       	cout << "Count + 1 success " << endl;
       else if(flag.compare("Reduce") == 0)
       	cout << "Count - 1 success " << endl;
    }
}

int connectMysql::Insert(map<string,string>  mapped_v)
{
    string Table_Name = mapped_v["Table_Name"];
    string Fuid = mapped_v["Fuid"];
    string value = mapped_v["flag"];
    string Type;
    string Column;

    if(Table_Name.compare("t_relation") == 0)
    {
       string Column = "Fqqid";
       string Fsign = mapped_v["sign"];
       Insert_t_relation("t_relation",Fuid,value,Fsign);		//input to t_relation
    }
    else
    {
       if(Table_Name.compare("bankid_statistics") == 0)
       {
			Column = "Fbankid";
       }
       else if(Table_Name.compare("creid_statistics") == 0)
	   {
			Column = "Fcreid";
	   }
       else if(Table_Name.compare("mobile_statistics") == 0)
	   {
			Column = "Fmobile";
	   }
       Deal_Insert(Table_Name,Fuid,Column,value);
    }
}

int connectMysql::Insert_t_relation(const string & DB_Name,
								    const string & Fuid,
   							    	const string & Fqqid,
   						        	const string & sign)
{
    int ret;
	string sql;
	stringstream ss;

	ss <<"insert into "<<DB_Name<<"(Fuid,Fqqid,Fsign) values ("<<Fuid<<","
						<<Fqqid<<","<<sign<<");"<<endl;
	sql = ss.str();
    cout << "sql t_relation is " << sql << endl;

	conn->Mysql_Query(sql,sql.length());
}

int connectMysql::Deal_Insert(const string &Table_Name,const string &Fuid,
								const string &Column,const string &value)
{
	string sql;
	stringstream ss;
	ss << "select count(*) from "<< Table_Name<<" where "<<Column<<"="
														<<value<<";"<<endl;
	sql = ss.str();
	string result = conn ->Handle_Sql(sql);
   // string result = conn->Select(Table_Name,Column,value,"Count");
    if(result.compare("1") == 0)
    {
       cout << "The Column exist " << endl;
       //need count add 1
       Count_AddOrReduce_One(Table_Name,Column,value,"Add");
    }
    else
    {
	   stringstream ss;
	   string sql;
		
	   ss <<"insert into "<<Table_Name<<"("<<Column<<",Count) values(" 
													<<value<<",1);"<<endl;
	   sql = ss.str();
       cout << "insert is : " << sql << endl;
	
	   conn->Mysql_Query(sql,sql.length());
    }
    Update_User_info_backup(Table_Name,Fuid,Column,value);
}

/*------------------------Delete not t_relation--------------------------------------*/
int connectMysql::Update_User_info_backup(const string &Table_Name,const string &Fuid,
											const string &Column,const string &value)
{
	string sql;
	stringstream ss;
	ss <<"select count(*) from " <<Table_Name<<" where "<<Column<<"="<<value<<";"<<endl;
	sql = ss.str();


   // string ret = conn->Select("user_info_backup","Fuid",Fuid,"Count");
    string ret = conn -> Handle_Sql(sql);
    if(ret.compare("1") == 0)
    {
       cout << "heere run ?" << endl;
       Update_Column("user_info_backup",Fuid,Column,value);
    }
    else
    {
       Insert_Column("user_info_backup",Fuid,Column,value,"user_info_backup");
    }
}

int connectMysql::Update_Column(const string &Table_Name,const string &Fuid,
								const string &Column,const string &value)
{
    int ret;
	stringstream ss;
	string sql;
    
	cout << "exist in " << Table_Name  << endl;
	ss <<"update user_info_backup set "<<Column<<"="<<value<<" where Fuid="
														<<Fuid<<";"<<endl;
	sql = ss.str();
    
	cout << "update is : " << sql << endl;
	conn->Mysql_Query(sql,sql.length());
}

int connectMysql::Insert_Column(const string &Table_Name,
								const string &Fuid,
								const string &Column,
								const string &value,
								const string &flag)
{
    int ret;
	string sql;
    stringstream ss;
    if(flag.compare("user_info_backup") == 0)
    {
       cout << "unexit in user_info_backup" << endl;
       //unexist ,so insert into user_info_backup
	   ss << "insert into user_info_backup (Fuid,"<<Column <<") values ("
						<<Fuid<<","<<value<<");" << endl;
	   sql = ss.str();
    }
    else if(flag.compare("statistics") == 0)
    {
       cout << "unexist in " << Table_Name << endl;
	   
	   ss <<"insert into " <<Table_Name<<" ("<<Column<<",Count) values ("
											<<value<<",1);"<<endl;
       cout << "insert column is :" << sql << endl;
    }
    

	conn->Mysql_Query(sql,sql.length());
}

/*Delete*/
int connectMysql::Set_Column_NULL(const string &Fuid,const string &Column)
{
	string sql;
    stringstream ss;
    ss << "update user_info_backup set "<< Column << " =NULL where Fuid="
													<<Fuid<<";" << endl;
    sql = ss.str();
    cout << "sql = " << sql << endl;

	conn->Mysql_Query(sql,sql.length());
}

int connectMysql::Delete_One_Record(const string &Table_Name,const string &Fuid)
{
	int ret;
	string sql;
    stringstream ss;

    ss << "delete from "<< Table_Name <<" where Fuid=" << Fuid <<";" << endl;
    sql = ss.str();
    cout << "sql = " << sql << endl;

	ret =  conn->Mysql_Query(sql,sql.length());
	if(ret == 0)
       cout << "delete a record success " << endl;
}

int connectMysql::Deal_Delete(const string &Table_Name,const string &Fuid,
							  const string &Column,const string &value)
{
	string sql;
	stringstream ss;

	ss<<"select "<<Column<<" from user_info_backup where Fuid="<<Fuid<<";"<<endl;
	sql = ss.str();
   // string result = conn->Select("user_info_backup",Column,Fuid,"Delete");
    string result = conn->Handle_Sql(sql);
	if(result.length() != 0)
    {
       cout << "The Column exist " << endl;
       cout << "result Column is " << result << endl;

       Count_AddOrReduce_One(Table_Name,Column,result,"Reduce");
       //set user_info_backup column as NULL
       Set_Column_NULL(Fuid,Column);
    }
    else
    {
       cout << "The Colmun unexist" << endl;
    }

    /*if all column is null */
	string sq;
	stringstream s;

	s<<" select count(*) from user_info_backup where Fmobile is null and "
						<<" Fcreid is null and Fbankid is null;"<<endl;

	result = conn->Handle_Sql(sq);
   // result = conn->Select("user_info_backup",Column,Fuid,"All");
    cout << "result in delete :" << result << endl;
    if(result.compare("1") != 0)
	{
       cout << "user_info_back is not NULL,so ,dot need delete user" << endl;
	}
    else
    {
       cout << "I will delete a record " << endl;
       //delete a record
       Delete_One_Record("user_info_backup",Fuid);
    }
}
/*-------------------------------------------------------------------------------*/

/*---------------------------------Delete t_relation---------------------------------------------*/
int connectMysql::Delete_t_relation(const string &Fqqid)
{
    string Fuid;
    string Fmobile;
    string Fcreid;
    string Fbankid;
	string sql;
	stringstream ss;

	ss<<"select Fuid from t_relation  where Fqqid="<<Fqqid<<";"<<endl;
	sql = ss.str();

   // Fuid = conn->Select("t_relation","Fuid",Fqqid,"DR");
    Fuid = conn->Handle_Sql(sql);
	if(Fuid.length() == 0)
       cout << "delete Fqqid is not exist" << endl;
    else
       cout << "Fuid =" << Fuid << endl;

	ss.clear();
	ss<<"select Fmobile  from user_info_backup  where Fuid="<<Fuid<<";"<<endl;
	sql = ss.str();
 //   Fmobile = conn->Select("user_info_backup","Fmobile",Fuid,"Delete");
	Fmobile = conn->Handle_Sql(sql);
	if(Fmobile.length() == 0)
       cout << "Fmobile is not exist" << endl;
    else
       Count_AddOrReduce_One("mobile_statistics","Fmobile",Fmobile,"Reduce");

	ss.clear();
	ss<<"select Fcreid from user_info_backup where Fuid="<<Fuid<<";"<<endl;
	sql = ss.str();
   // Fcreid = conn->Select("user_info_backup","Fcreid",Fuid,"Delete");
    Fcreid = conn->Handle_Sql(sql);
	if(Fcreid.length() == 0)
       cout << "Fcreid is not exist" << endl;
    else
       Count_AddOrReduce_One("creid_statistics","Fcreid",Fcreid,"Reduce");

	ss.clear();
	ss<<"select Fbankid from user_info_backup where Fuid="<<Fuid<<";"<<endl;
	sql = ss.str();

   // Fbankid = conn->Select("user_info_backup","Fbankid",Fuid,"Delete");
    Fbankid = conn->Handle_Sql(sql);
	if(Fbankid.length() == 0)
       cout << "Fbankid is not exist" << endl;
    else
       Count_AddOrReduce_One("bankid_statistics","Fbankid",Fbankid,"Reduce");
}
/*-----------------------------------------------------------------------------------------------*/
int connectMysql::Delete(map<string,string> mapped_v)
{
    string Table_Name = mapped_v["Table_Name"];
    string Fuid = mapped_v["Fuid"];
    string value = mapped_v["flag"];
    string Type;
    string Column;

    if(Table_Name.compare("t_relation") == 0)
    {
       string Column = "Fqqid";
       string Fsign = mapped_v["sign"];
       
       cout << "Fqqid =" << value << endl;

       //delete table t_relation
       Delete_t_relation(value);
       
    }
    else
    {
       if(Table_Name.compare("bankid_statistics") == 0)
       {
       	Column = "Fbankid";
       }
       else if(Table_Name.compare("creid_statistics") == 0)
       	Column = "Fcreid";
       else if(Table_Name.compare("mobile_statistics") == 0)
       	Column = "Fmobile";
       Deal_Delete(Table_Name,Fuid,Column,value);
    }
}

/*-------------------------------------------Update----------------------------------------------------*/
int connectMysql::Deal_Update(const string &Table_Name,const string &Fuid,
								const string &Column,const string &value)
{
    /*
     * 1 find the column in backup
     * 2 count - 1 in statictisc_table
     * 3 if new value in statictisc,add 1 ,else insert
     * 4 update column in user_backup
     * */
	string sql;
	stringstream ss;

	ss<<"select "<<Column<<" from user_info_backup where Fuid="<<Fuid<<";"<<endl;
	sql = ss.str();

    string result = conn->Handle_Sql(sql);
    //result = conn->Select("user_info_backup",Column,Fuid,"Delete");   //1
    cout << "result =" << result << endl;

    Count_AddOrReduce_One(Table_Name,Column,result,"Reduce");   //2

	ss.clear();
	ss<<"select count(*) from "<<Table_Name<<" where "<<Column<<"="<<value<<";"<<endl;
	sql = ss.str();
	result = conn->Handle_Sql(sql);
 //   result = conn->Select(Table_Name,Column,value,"Count");   		//3
    cout << "result in yuanshibiao:" << result << endl;
    if(result.compare("0") == 0)   		
    {
       cout << "the new column not exist in statistics table" << endl;
       Insert_Column(Table_Name,Fuid,Column,value,"statistics");
    }
    else
    {
       cout << "the new column exist in statistics table" << endl;
       Count_AddOrReduce_One(Table_Name,Column,value,"Add");
    }

    Update_Column("uesr_info_backup",Fuid,Column,value);   	//4

}

int connectMysql::Update_Column_t_relation(const string &Fuid,const string &Fsign)
{

    int ret;
    cout << "exist in  t_relation " << endl;
    //exist update user_info_backup

    stringstream ss;
    ss << "update t_relation set Fsign=" << Fsign.c_str()<<" where Fuid=" 
											<<Fuid.c_str()<<";" << endl;
    string sql = ss.str();
	cout << "update column is :" << sql << endl;
   
	ret =  conn->Mysql_Query(sql,sql.length());
}

int connectMysql::Chenge_Column(const string &Fmobile,const string &Fcreid,
													const string &Fbankid,
													const string &Fuid,
													const string &flag)
{
	string count;
	string sql;
	stringstream ss;

	ss<<"select count(*) from creid_statistics where Fcreid="<<Fcreid<<";"<<endl;
	sql = ss.str();

	count = conn->Handle_Sql(sql);
//	count = conn->Select("creid_statistics","Fcreid",Fcreid,"Count");// 2
    if(count.compare("1") == 0)
	{
		Count_AddOrReduce_One("creid_statistics","Fcreid",Fcreid,flag);
	}
	else
	{
	    cout << "insert ?" << endl;
		Insert_Column("creid_statistics",Fuid,"Fcreid",Fcreid,"statistics");
	}
	 
	ss.clear();
	ss<<"select count(*) from mobile_statistics where Fmobile="<<Fmobile<<";"<<endl;
	sql = ss.str();
//	count = conn->Select("mobile_statistics","Fmobile",Fmobile,"Count");// 2
	count = conn->Handle_Sql(sql);
	if(count.compare("1") == 0)
		Count_AddOrReduce_One("mobile_statistics","Fmobile",Fmobile,flag);
	else
		Insert_Column("mobile_statistics",Fuid,"Fmobile",Fmobile,"statistics");


	ss.clear();
	ss<<"select count(*) from bankid_statistics where Fbankid="<<Fbankid<<";"<<endl;
	sql = ss.str();

	count = conn -> Handle_Sql(sql);
	//count = conn->Select("bankid_statistics","Fbankid",Fbankid,"Count");// 2
	if(count.compare("1") == 0)
		Count_AddOrReduce_One("bankid_statistics","Fbankid",Fbankid,flag);
    else 
		Insert_Column("bankid_statistics",Fuid,"Fbankid",Fbankid,"statistics");
       
}
int connectMysql::Deal_Update_t_relation(const string &Fqqid,const string &Fsign)
{

    /*
     *1 Fqqid  ---> Fuid
     *2 Fuid   ---> Fmobile,Fcreid,Fbankid
     *3 if Fsigb  1->2    (Fmobile,Fcreid,Fbankid) - 1;
     *  else 2->1   	if(Fmobile,Fcreid,Fbankid) exist , + 1
     *   					else  insert 
     * */
    string Fuid,sign,Fmobile,Fcreid,Fbankid;
	string sql;
	stringstream ss;

	ss<<"select Fuid from t_relation where Fqqid="<<Fqqid<<";"<<endl;
	sql = ss.str();
	Fuid = conn->Handle_Sql(sql);
    //Fuid = this->conn->Select("t_relation","Fuid",Fqqid,"DR");   		// 1
    cout <<"t_relation Fuid ="  << Fuid << endl;
    
	ss.clear();
	ss<<"select Fsign from t_relation where Fqqid="<<Fqqid<<";"<<endl;
	sql = ss.str();
	sign = conn->Handle_Sql(sql);
   // sign = conn->Select("t_relation","Fsign",Fqqid,"DR");
    cout << "t_relation Fsign =" << sign << endl;

	ss.clear();
	ss<<"select Fmoblie from user_info_backup where Fuid="<<Fuid<<";"<<endl;
	sql = ss.str();
	Fmobile = conn->Handle_Sql(sql);
//    Fmobile = conn->Select("user_info_backup","Fmobile",Fuid,"Delete");// 2
    cout << "user_info Fmobile =" << Fmobile << endl;
    
	ss.clear();
	ss<<"select Fcreid from user_info_backup where Fuid="<<Fuid<<";"<<endl;
	sql = ss.str();
	Fcreid = conn->Handle_Sql(sql);
//    Fcreid = conn->Select("user_info_backup","Fcreid",Fuid,"Delete");
    if((Fsign.compare("2\n") == 0) && (sign.compare("1") ==0))   // 3 if
    {
    
       Chenge_Column(Fmobile,Fcreid,Fbankid,Fuid,"Reduce");
       Update_Column_t_relation(Fuid,"2");	
    }
    else if((Fsign.compare("1\n") == 0) && (sign.compare("2") == 0))
    {
       Chenge_Column(Fmobile,Fcreid,Fbankid,Fuid,"Add");
       Update_Column_t_relation(Fuid,"1");	
       
    }
}

int connectMysql::Update(map<string,string> mapped_v)
{

    string Table_Name = mapped_v["Table_Name"];
    string Fuid = mapped_v["Fuid"];
    string value = mapped_v["flag"];
    string Type;
    string Column;

    if(Table_Name.compare("t_relation") == 0)
    {
       string Column = "Fqqid";
       string Fsign = mapped_v["sign"];
       
       cout << "Fqqid =" << value << endl;

    //   update t_relation table
       Deal_Update_t_relation(value,Fsign);
    }
    else
    {
       if(Table_Name.compare("bankid_statistics") == 0)
       {
       	Column = "Fbankid";
       }
       else if(Table_Name.compare("creid_statistics") == 0)
       	Column = "Fcreid";
       else if(Table_Name.compare("mobile_statistics") == 0)
       	Column = "Fmobile";

       Deal_Update(Table_Name,Fuid,Column,value);
    }
}
/*-----------------------------------------------------------------------------------------------------*/

/*********************************************Replace**********************************************************/
int connectMysql::Deal_Replace_t_relation(const string &Fuid,const string &Fqqid,
															const string &Fsign)
{
    int ret;
	string result;
    stringstream ss;

    ss << "insert into t_relation (Fuid,Fqqid,Fsign)  values (" << Fuid.c_str() 
					<<"," << Fqqid.c_str() << "," <<Fsign.c_str() <<");" << endl;
    string sql = ss.str();
	cout << "insert column is :" << sql << endl;
   
	ret =  conn->Mysql_Query(sql,sql.length());
}

int connectMysql::Deal_Replace(const string & Table_Name,const string & Fuid,
				const string &Column,const string & value)
{
	//1 find Column in table?
	//if in  count + 1
	//not ,insert a record
	int ret;
	string result;
	string sql;
	stringstream ss;

	ss<<"select count(*) from "<<Table_Name<<" where "<<Column<<"="<<value<<";"<<endl;
	sql = ss.str();
	result = conn->Handle_Sql(sql);
//	result = conn->Select(Table_Name,Column,value,"Count");
	if(result.compare("1") == 0)
	{
		Count_AddOrReduce_One(Table_Name,Column,value,"Add");

	}
	else
	{
		Insert_Column(Table_Name,Fuid,Column,value,"statistics");
	}

	Update_Column("user_info_backup",Fuid,Column,value);
}

int connectMysql::Replace(map<string,string> mapped_v)
{

    string Table_Name = mapped_v["Table_Name"];
    string Fuid = mapped_v["Fuid"];
    string value = mapped_v["flag"];
    string Type;
    string Column;

    if(Table_Name.compare("t_relation") == 0)
    {
       string Column = "Fqqid";
       string Fsign = mapped_v["sign"];
       
       cout << "Fqqid =" << value << endl;

    //   update t_relation table
       Deal_Replace_t_relation(Fuid,value,Fsign);
    }
    else
    {
       if(Table_Name.compare("bankid_statistics") == 0)
       {
			Column = "Fbankid";
       }
       else if(Table_Name.compare("creid_statistics") == 0)
	   {
			Column = "Fcreid";
	   }
       else if(Table_Name.compare("mobile_statistics") == 0)
	   {
			Column = "Fmobile";
	   }

       Deal_Replace(Table_Name,Fuid,Column,value);
    }
}
/*******************************************************************************************************/
