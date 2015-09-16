BIN_DTR = ../bin
LIB_FLAG = -std=gnu++0x -L/usr/lib/mysql -lmysqlclient -lpthread -ldl
INCLUDE_FLAG = -I ../include -I./usr/local/include -I/usr/include/mysql/

Client : Client.o clientConnect.o 
	g++ -o -Wall -ggdb -o $@ $^ ${LIB_FLAG}

Server : Server.o serverConnect.o connectmysql.o  pool.o
	g++ -o -Wall -ggdb -o $@ $^ ${LIB_FLAG}

%.o : %.cpp
	g++ -g -o $@ ${INCLUDE_FLAG} -c $^

clean :
	rm -f *.o 

