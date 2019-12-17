#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>

using namespace std;

#ifndef bufsize
#define bufsize 30
#endif

void error_handle( string str ) {
	cout << str << endl;
	exit(1);
}

int main(int argc, char* argv[]){
	int serv_sock, clnt_sock;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;

	int str_len;

	char message[bufsize];

	if (argc != 2) {
		cout << "Usage : " << argv[0] << " <port> " << endl;
		return -1;
	}

	serv_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1){
		error_handle("Create server socket failed ...");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	int ret = bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if (ret == -1) {
		error_handle("bind failed ...");
	}

	ret = listen(serv_sock, 5);
	if (ret == -1) {
		error_handle("listen failed ...");
	}

	socklen_t clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	if (clnt_sock == -1) {
		error_handle("accept failed ...");
	}

	FILE* fp = fopen("data.txt", "r+");
	if (fp == NULL) {
		error_handle("Open file failed ...");
	}

	while(1) {
		str_len = fread((void *)message, 1, sizeof(message), fp);
		cout << message << endl;
		if (str_len < bufsize) {
			write(clnt_sock, message, str_len);
			break;
		}
		write(clnt_sock, message, sizeof(message));
		memset(message, 0, sizeof(message));
	}

	shutdown(clnt_sock, SHUT_WR);
	str_len = read(clnt_sock, message, sizeof(message));
	if (str_len > 0)
		cout << message << endl;

	fclose(fp);
	close(clnt_sock);
	close(serv_sock);
	return 0;
}