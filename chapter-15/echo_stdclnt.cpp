#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

#ifndef bufsize
#define bufsize 1024
#endif

void error_handle( string str ) {
	cout << str << endl;
	exit(1);
}

int main(int argc, char* argv[]) {
	struct sockaddr_in serv_addr;
	char message[bufsize];
	int str_len;

	FILE *readfp;
	FILE *writefp;

	if (argc != 3) {
		cout << "Usage : " << argv[0] << " <IP> <port>" << endl;
		return -1;
	}

	int clnt_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (clnt_sock == -1){
		cout << "Create client socket failed ..." << endl;
		return -1;
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	socklen_t serv_addr_len = sizeof(serv_addr);
	int ret = connect(clnt_sock, (struct sockaddr*)&serv_addr, serv_addr_len);
	if (ret == -1) {
		cout << "connect failed ..." << endl;
		return -1;
	}

	readfp = fdopen(clnt_sock, "r");
	writefp = fdopen(clnt_sock, "w");

	memset(message, 0, sizeof(message));
	while(1) {
		cout << "input \"Q\" or \"q\" to quit" << endl;
		fgets(message, sizeof(message), stdin);
		if ( !strcmp(message, "Q") || !strcmp(message, "q") ) {
			cout << "quit !!!" << endl;
			break;
		}

		fputs(message, writefp);
		fflush(writefp);
		memset(message, 0, sizeof(message));
		fgets(message, sizeof(message), readfp);

		cout << "message from server: " << message << endl;
	}

	fclose(writefp);
	fclose(readfp);
	return 0;
}