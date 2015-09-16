/*************************************************************************
	> File Name: Coonnect.cpp
	> Author: LCG
	> Mail: lichungang521521.com
	> Created Time: Fri 07 Aug 2015 02:24:21 AM PDT
 ************************************************************************/
#include"Client.h"
#include"Server.h"
#include"connectmysql.h"

int RecvData(const int &ClientFd)
{
	 /*recv two times 
	 *first recv data length
	 *second recv true data
	 * */
	char buf[1000];
	char buff[4];
	int length;
	int rev;
	memset(buf,0,1000);
	memset(buff,0,4);

	rev = recv(ClientFd,buff,sizeof(buff),0);
	if(rev <= 0)
		return -1;
	length = atoi(buff);
	cout << "buf size is : " << length << endl;

	sprintf(buff,"%s","y");
	rev = send(ClientFd,buff,strlen(buff),0);
	assert(rev >= 0);

	rev = recv(ClientFd,buf,length,0);
	buf[length] = '\0';
	cout << "rev real data is " << buf<<endl;

	close(ClientFd);

	/*send a */
	DataProcessHelper DPH;
    DPH.Initial(buf);

	return 0;
}

/***************ClientAcceptor********************/
int ClientAcceptor::Epoll_Register(const int &epollfd,epoll_event * events)
{
	while(1)
	{
		int ret = epoll_wait(epollfd,events,MAX_EVENT_NUMBER,-1);
		if(ret < 0)
		{
			cerr<<"epoll failure"<<endl;
			break;
		}

		for(int i=0; i<ret; i++)
		{
			int sockfd = events[i].data.fd;
			if(sockfd == SocketFd)
			{
				struct sockaddr_in cliaddr;
				socklen_t clilen = sizeof(cliaddr);

				int connfd = accept(SocketFd,(struct sockaddr*)&cliaddr,&clilen);
				Addfd(epollfd,connfd,true);
			}
			else if(events[i].events & EPOLLIN)
			{
				pthread_t pthread;
				struct fds fds_worker;
				
				fds_worker.epollfd = epollfd;
				fds_worker.sockfd = sockfd;

				pthread_create(&pthread,NULL,worker,(void *)&fds_worker);
			}
			else
			{
				cout<<"something else happened"<<endl; 
			}
		}
	}

	close(SocketFd);		//server shutdown
	return 0;
}


ClientAcceptor::ClientAcceptor()
{

	SocketFd = socket(AF_INET,SOCK_STREAM,0);
	if(SocketFd < 0)
	{
		cerr<<"obtain socketfd error"<<endl;
		return ;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(SERVER_PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int ret = bind(SocketFd,(struct sockaddr*)&addr,sizeof(addr));
	if(ret < 0)
	{
		cerr<<"bind port and address error"<<endl;
		return ;
	}

	ret = listen(SocketFd,10);
	assert(ret >= 0);

	epoll_event events[MAX_EVENT_NUMBER];
	int epollfd = epoll_create(5);
	assert(epollfd != -1);

	Addfd(epollfd,SocketFd,false);

	Epoll_Register(epollfd,events);
}

ClientAcceptor::~ClientAcceptor()
{
}
/*epoll*/
void ClientAcceptor::Addfd(const int &epollfd,const int &fd,const bool &flag)
{
	epoll_event event;
	event.data.fd = fd;
	event.events = EPOLLIN | EPOLLET;

	if(flag)		//if open ET motion
	{
		event.events |= EPOLLONESHOT;
	}
	epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);

}

void ClientAcceptor::reset_oneshot(const int &epollfd,const int &fd)
{
	epoll_event event;
	event.data.fd =fd;
	event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
	epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&event);
}

void * worker(void *arg)
{
	/*worker pthread*/
	int connfd = ((struct fds*)arg)->sockfd;
	int epollfd = ((struct fds*)arg) -> epollfd;

	/**/

	RecvData(connfd);

}

int DataProcessHelper::CallHandleFunction(map<string,string> mapped_v)
{
	connectMysql mq(mapped_v);
	if(mapped_v["DML_type"] == "1")
		mq.Insert(mapped_v);
	else if(mapped_v["DML_type"] == "2")
		mq.Update(mapped_v);
	else if(mapped_v["DML_type"] == "3")
		mq.Delete(mapped_v);
	else
		mq.Replace(mapped_v);

}

int  DataProcessHelper::Initial(const string & Buffer)
{
	map<string,string> mapped_v;
	char *ptr = (char *)Buffer.c_str();
	cout << "I will handle buffer" << Buffer << endl;
		
	char *token = strtok(ptr,"&");
	while(token != NULL)
	{
		char *p = token;
		string index;
		string value;
		while(*p != '=')
		{
			index = index + *p;
			p++;
		}

		p++;
		while(*p != '\0')
		{
			value = value + *p;
			p++;
		}

		if(index.compare("Fcreid") == 0 || index.compare("Fmobile") 
				== 0 || index.compare("Fbankid") == 0 || index.compare("Fqqid") == 0)
		{
			index = "flag";
		}
		else if(index.compare("Fsign") == 0)
		{
			index = "sign";
		}
		
		cout << "index = " << index << endl;
		mapped_v.insert(pair<string,string>(index,value));
		token = strtok(NULL,"&");
	}

	/*call function,*/
	CallHandleFunction(mapped_v);
}
