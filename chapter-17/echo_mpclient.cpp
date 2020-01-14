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

void read_routine(int sock, char *buf) {
	while (1) {
		int str_len = read(sock, buf, sizeof(buf));
		if (str_len == 0)
			return;
		cout << buf << endl;
		memset(buf, 0, sizeof(buf));
	}
}

void write_routine(int sock, char *buf) {
	while (1) {
		cin >> buf;
		write(sock, buf, strlen(buf));
		if ( !strcmp(buf, "Q") || !strcmp(buf, "q") ) {
			cout << "quit !!!" << endl;
			return;
		}
		memset(buf, 0, sizeof(buf));
	}
}

void error_handle( string str ) {
	cout << str << endl;
	exit(1);
}

int main(int argc, char* argv[]) {
	struct sockaddr_in serv_addr;
	pid_t pid;
	int str_len;
	char buf[BUF_SIZE];

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
		error_handle("connect failed ...");
	}

	pid = fork();
	if (pid == 0) {
		write_routine(clnt_sock, buf);
	}
	else {
		read_routine(clnt_sock, buf);	
	}

	close(clnt_sock);
	return 0;
}