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
	char message[bufsize];
	int str_len;
	int rec_len = 0;
	socklen_t addr_sz;

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

	char quit_flag1[] = "q from server";
	char quit_flag2[] = "Q from server";

	while (1) {
		cout << "Input: ";
		cin >> message;

		addr_sz = sizeof(serv_addr);
		sendto(clnt_sock, message, sizeof(message), MSG_NOSIGNAL, (struct sockaddr*)&serv_addr,  addr_sz);
		addr_sz = sizeof(from_adr);
		str_len = recvfrom(clnt_sock, message, sizeof(message), 0, (struct sockaddr*)&from_adr, &addr_sz);
		cout << "recv: " << message << endl;
		if (!strcmp(message, quit_flag1) || !strcmp(message, quit_flag2)) {
			cout << "quit ..." << endl;
			break;
		}
		memset(message, 0, sizeof(message));
	}

	close(clnt_sock);	
	return 0;
}