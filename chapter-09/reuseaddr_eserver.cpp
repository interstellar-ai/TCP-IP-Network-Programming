#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

using namespace std;

void error_handle( string str ) {
	cout << str << endl;
	exit(1);
}

int main(int argc, char const *argv[])
{
	int serv_sock, clnt_sock;
	char message[30];
	int option, str_len;
	socklen_t optlen, clnt_addr_sz;
	struct sockaddr_in serv_addr, clnt_addr;

	if (argc != 2) {
		cout << "Usage : " << argv[0] << " <port>" << endl;
		return -1;
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1) {
		error_handle("socket() errror");
	}

	optlen = sizeof(option);
	option = true;
	setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&option, optlen);

	memset(&serv_addr, 0 ,sizeof(serv_addr));
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

	clnt_addr_sz = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_sz);
	if (clnt_sock == -1) {
		error_handle("accept failed ...");
	}

	while( str_len = read(clnt_sock, message, sizeof(message)) != 0) {
		write(clnt_sock, message, 	str_len);
		// write(1, message, str_len);
		cout << message << endl;
	}

	close(clnt_sock);
	close(serv_sock);

	return 0;
}