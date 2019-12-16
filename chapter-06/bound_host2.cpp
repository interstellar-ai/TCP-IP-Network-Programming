#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

#ifndef bufsize
#define bufsize 30
#endif

void error_handle( string str ) {
	cout << str << endl;
	exit(1);
}

int main(int argc, char* argv[]) {
	struct sockaddr_in serv_addr, from_adr;
	int str_len;
	int rec_len = 0;
	socklen_t addr_sz;

	char message1[] = "hello world";
	char message2[] = "how are you";
	char message3[] = "I love you";

	if (argc != 3) {
		cout << "Usage : " << argv[0] << " <IP> <port>" << endl;
		return -1;
	}

	int clnt_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (clnt_sock == -1){
		error_handle("Create socket failed ...");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	addr_sz = sizeof(serv_addr);
	sendto(clnt_sock, message1, sizeof(message1), MSG_NOSIGNAL, (struct sockaddr*)&serv_addr,  addr_sz);
	sendto(clnt_sock, message2, sizeof(message2), MSG_NOSIGNAL, (struct sockaddr*)&serv_addr,  addr_sz);
	sendto(clnt_sock, message3, sizeof(message3), MSG_NOSIGNAL, (struct sockaddr*)&serv_addr,  addr_sz);

	close(clnt_sock);	
	return 0;
}