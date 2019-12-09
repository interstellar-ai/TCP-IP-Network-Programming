#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

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
	int rec_len = 0;

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
	memset(message, 0, sizeof(message));
	cout << "sizeof message: " << sizeof(message) << endl;
	while(1) {
		cout << "input \"Q\" or \"q\" to quit" << endl;
		cin >> message;
		if ( !strcmp(message, "Q") || !strcmp(message, "q") ) {
			cout << "quit !!!" << endl;
			break;
		}

		int str_write_len = write(clnt_sock, message, sizeof(message));
		if (str_write_len == -1) {
			error_handle("write() error");
		}
		cout << "str_write_len: " << str_write_len << endl;
		rec_len = 0;
		while (rec_len < str_write_len) {
			str_len = read(clnt_sock, &message[rec_len], sizeof(message) - 1);
			if (str_len == -1) {
				error_handle("read() error");
			}
			rec_len += str_len;
		}
		cout << "rec_len: " << rec_len << endl;
		cout << "message from server: " << message << endl;
		memset(message, 0, sizeof(message));
	}

	close(clnt_sock);
	return 0;
}