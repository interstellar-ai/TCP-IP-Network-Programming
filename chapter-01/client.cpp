#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[]) {
	struct sockaddr_in serv_addr;
	char message[30];
	int str_len;

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

	str_len = read(clnt_sock, message, sizeof(message) - 1);
	if (str_len == -1) {
		cout << "read failed ..." << endl;
		return -1;
	}

	cout << "message from server: " << message << endl;
	close(clnt_sock);
	return 0;
}