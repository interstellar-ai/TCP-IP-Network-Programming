#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

using namespace std;

#ifndef bufsize
#define bufsize 30
#endif

void error_handle( string str ) {
	cout << str << endl;
	exit(1);
}

int main(int argc, char* argv[]){
	int serv_sock;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;

	socklen_t clnt_adr_sz;
	int str_len;

	char message[bufsize];

	if (argc != 2) {
		cout << "Usage : " << argv[0] << " <port> " << endl;
		return -1;
	}

	serv_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (serv_sock == -1){
		error_handle("Create socker failed ...");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	int ret = bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if (ret == -1) {
		error_handle("bind failed ...");
	}

	for (int i = 0; i < 3; i++) {
		sleep(5);
		clnt_adr_sz = sizeof(clnt_addr);
		str_len = recvfrom(serv_sock, message, bufsize, 0, (struct sockaddr*)&clnt_addr, &clnt_adr_sz);
		if (str_len > 0) {
			cout << message << endl;
		}
	}
	close(serv_sock);
	return 0;
}