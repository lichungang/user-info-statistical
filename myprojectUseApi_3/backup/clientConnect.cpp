/*************************************************************************
	> File Name: Coonnect.cpp
	> Author: LCG
	> Mail: lichungang521521.com
	> Created Time: Fri 07 Aug 2015 02:24:21 AM PDT
 ************************************************************************/
#include"Client.h"
#include"Server.h"

/*client*/

//int Client::handle_input(ACE_HANDLE fd)
/**/
Client::Client()
{
	SocketFd = socket(AF_INET,SOCK_STREAM,0);
	if(SocketFd < 0)
	{
		cerr << "obtain socketfd error";
		return ;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(CLIENT_PORT);
	addr.sin_addr.s_addr = inet_addr(CLIENT_ADDR);

	int ret = connect(SocketFd,(struct sockaddr*)&addr,sizeof(addr));
	if(ret < 0)
	{
		cerr << "connecting server error" << endl;
		return ;
	}
		
}

Client::~Client()
{
}

int Client::SendTrueData()
{
	
	int ret;
	char buffer[10];
	//peer.recv(buffer,10);
	ret = recv(SocketFd,buffer,sizeof(buffer),0);
	if(ret < 0)
	{
		cerr<<"recv data error "<<endl;
	}

	char *p = (char*)readData.c_str();
   // ret = peer.send(p,length);
	ret = send(SocketFd,p,length,0);
	if(ret < 0)
	{
		cout << "send data error" << endl;
		return -1;
	}
	cout << "send data length = " << length << endl;
	cout << "send data is :" <<endl;
	cout << readData << endl;

	return 0;
}



string Client::Split(string str,Send_Type file,string buffer)
{
	int flag = 0;
	char *p;
	char* s = (char *)buffer.c_str();
	const char *split = "#";
	p = strtok(s,split);
	while(p != NULL)
	{
		flag++;
		if(flag == 1)
		{
			str = str + p;		//p DB_name;
			str = str + "&Table_Name=";
		}
		else if(flag == 2)
		{
			str = str + p;		//Table_Name
			str = str + "&Fuid=";
		}
		else if(flag == 3 && file == 3)
		{
			str = str + p;		//Fuid
			str = str + "&Fcred=";
		}
		else if(flag == 3 && file == 4)
		{
			str = str + p;		//Fcred
			str = str + "&Fbankid=";
		}
		else if(flag == 3 && file == 2)
		{
			str = str + p;		//Fbankid
			str = str + "&Fmobile=";
		}
		else if(flag == 3 && file == 5)
		{
			str = str + p;		//Fmobile
			str = str + "&Fqqid=";
		}
		else if(flag == 4)
		{
			str = str + p;		//Fqqid
		}
		else if(flag == 5)
		{
			str = str + "&Fsign=";
			str  = str + p;		//Fsign	
		}
		p = strtok(NULL,split);
	}
	return str;
}
string Client::FormData(DML_type d_type,string buffer,Send_Type file)
{
	string RealData;
	string str;
	
	char tmp[2];
	cout << "d_type = " << d_type << endl; 
	sprintf(tmp,"%d",d_type);
	tmp[2] = '\0';

	str = "DML_type=";
	str = str + tmp ;
	str = str + "&";
	str = str+"DB_name=";

	str = Split(str,file,buffer);
	RealData = str;
	return RealData;
}

string Client::ReadFile(DML_type d_type,string FileName,Send_Type file)
{
	string realData;
	ifstream rfile;
	cout <<"open file is : " << FileName << endl;
	char *ptr = (char *)FileName.c_str();
//	rfile.open("../cfg/t_user_info.txt",ifstream::in);
	rfile.open(ptr,ifstream::in);
	if(!rfile)
	{
		cout << "open error" << endl;
		return NULL;
	}

	rfile.seekg(0,ios::end);
	int length = rfile.tellg();
	rfile.seekg(0,ios::beg);
	char * buffer = new char[length];
	rfile.read(buffer,length);
	rfile.close();
	buffer[length] = '\0';

	
	realData = FormData(d_type,buffer,file);

	delete [] buffer;
	return realData;
}
int  Client::SendDataSize(DML_type d_type,Send_Type file)
{
	string FileName;
	char buf[4];
	
	cout << "file = " << file << endl;
	switch(file)
	{
		case 1:
			FileName = "../cfg/binlog.txt";
			break;
		case 2:
			FileName  = "../cfg/t_msgnotify_user.txt";
			break;
		case 3:
			FileName = "../cfg/t_user_info.txt";
			break;
		case 4:
			FileName = "../cfg/t_bank_user.txt";
			break;
		case 5:
			FileName = "../cfg/t_relation.txt";
			break;
	}

	readData = ReadFile(d_type,FileName,file);
	cout << "read send data is : " << readData << endl;
	
	length =readData.size();
	
	sprintf(buf,"%d",length);
	buf[4] = '\0';
	int ret;
//	ret = Peer().send(buf,4);//send tcp size to server
	ret = send(SocketFd,buf,sizeof(buf),0);
	if(ret < 0)
	{
		cout << "send data size error "<< endl;
	}
	else
		cout << "send data size is " << length << endl;


}

