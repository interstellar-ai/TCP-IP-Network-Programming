#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>


using namespace std;

#ifndef BUF_SIZE
#define BUF_SIZE 100
#endif

void error_handle( string str ) {
	cout << str << endl;
	exit(1);
}

int main(int argc, char const *argv[])
{
	struct sockaddr_in serv_addr;

	if (argc != 3) {
		cout << "Usage : " << argv[0] << " <IP> <port>" << endl;
		return -1;
	}

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if ( sock == -1 ){
		cout << "Create client socket failed ..." << endl;
		return -1;
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	socklen_t serv_addr_len = sizeof(serv_addr);
	int ret = connect(sock, (struct sockaddr*)&serv_addr, serv_addr_len);
	if (ret == -1) {
		error_handle("connect failed ...");
	}

	write(sock, "123", strlen("123"));
	send(sock, "4", strlen("4"), MSG_OOB);

	write(sock, "567", strlen("567"));
	send(sock, "890", strlen("890"), MSG_OOB);

	close(sock);
	return 0;
}