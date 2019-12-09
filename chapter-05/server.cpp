#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

using namespace std;

#ifndef bufsize
#define bufsize 1024
#endif

int main(int argc, char* argv[]){
	int serv_sock, clnt_sock;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;

	char message[bufsize];

	if (argc != 2) {
		cout << "Usage : " << argv[0] << " <port> " << endl;
		return -1;
	}

	serv_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1){
		cout << "Create server socket failed ..." << endl;
		return -1;
	}

	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	int ret = bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if (ret == -1) {
		cout << "bind failed ..." << endl;
		return -1;
	}

	ret = listen(serv_sock, 5);
	if (ret == -1) {
		cout << "listen failed ..." << endl;
		return -1;
	}

	socklen_t clnt_addr_size = sizeof(clnt_addr);
	char serv_flag[] = " from server";
	for(int i = 0; i < 5; i++) {
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
		if (clnt_sock == -1) {
			cout << "accept failed ..." << endl;
			return -1;
		}
		int str_len;
		memset(message, 0, sizeof(message));
		while ( str_len = read(clnt_sock, message, sizeof(message)) > 0) {
			strcat(message, serv_flag);
			write(clnt_sock, message, sizeof(message));
			cout << message << endl;	
		}
		close(clnt_sock);
	}

	close(serv_sock);
	return 0;
}