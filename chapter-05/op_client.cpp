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
	char opMessage[bufsize];
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
	memset(opMessage, 0, sizeof(opMessage));

	cout << "Operand count: " << endl;
	int operand_count;
	cin >> operand_count;
	opMessage[0] = char(operand_count);
	for (unsigned int i = 0; i < operand_count; i++) {
		cout << "Operand " << i + 1 << " : ";
		cin >> *((int *)&opMessage[i * sizeof(int) + 1]);
	}
	cout << "Operator: ";
	cin >> &opMessage[operand_count * sizeof(int) + 1];

	int str_write_len = write(clnt_sock, opMessage, operand_count * sizeof(int) + 2);
	if (str_write_len == -1) {
		error_handle("write() error");
	}

	char result[sizeof(int)];
	rec_len = 0;
	while (rec_len < sizeof(int)) {
		str_len = read(clnt_sock, &result[rec_len], sizeof(int));
		if (str_len == -1) {
			error_handle("read() error");
		}
		rec_len += str_len;
	}

	cout << "message from server: " << *((int *)&result) << endl;
	close(clnt_sock);	
	return 0;
}