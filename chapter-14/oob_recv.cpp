#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>

using namespace std;

unsigned int BUF_SIZE = 100;

int recv_sock, accept_sock;

void error_handle( string str ) {
	cout << str << endl;
	exit(1);
}

void urg_handler(int signo) {
	int str_len;
	char buf[BUF_SIZE];
	str_len = recv(recv_sock, buf, sizeof(buf) - 1, MSG_OOB);
	buf[BUF_SIZE] = 0;
	cout << "urg handler recv: " << buf << endl;
}

int main(int argc, char const *argv[])
{
	struct sockaddr_in serv_addr, recv_addr;
	int str_len, state;
	socklen_t server_adr_sz;
	char buf[BUF_SIZE];
	struct sigaction act;

	if (argc != 2) {
		cout << "Usage : " << argv[0] << " <port>" << endl;
		return -1;
	}

	accept_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (accept_sock == -1) {
		error_handle("socket() errror");
	}

	act.sa_handler = urg_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;


	memset(&recv_addr, 0 ,sizeof(recv_addr));
	recv_addr.sin_family = AF_INET;
	recv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	recv_addr.sin_port = htons(atoi(argv[1]));

	int ret = bind(accept_sock, (struct sockaddr *)&recv_addr, sizeof(recv_addr));
	if (ret == -1) {
		error_handle("bind failed ...");
	}
	ret = listen(accept_sock, 5);
	if (ret == -1) {
		error_handle("listen failed ...");
	}

	server_adr_sz = sizeof(serv_addr);
	recv_sock = accept(accept_sock, (struct sockaddr*)&serv_addr, &server_adr_sz);
	if (recv_sock == -1) {
		error_handle("accept failed ...");
	}

	fcntl(recv_sock, F_SETOWN, getpid());
	state = sigaction(SIGURG, &act, 0);

	while ((str_len = recv(recv_sock, buf, sizeof(buf), 0) ) != 0) {
		if (str_len == -1)
			continue;
		buf[str_len] = 0;
		cout << buf << endl;
	}

	close(recv_sock);
	close(accept_sock);
	return 0;
}