#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/socket.h>
#include "arpa/inet.h"

using namespace std;

#define BUF_SIZE 30

void read_childproc(int sig) {
	int status;
	pid_t pid = waitpid(-1, &status, WNOHANG);
	if (WIFEXITED(status)) {
		cout << "Remove proc id: " << pid << endl;
		cout << "Child send: " << WEXITSTATUS(status) << endl;
	}
}

void error_handle( string str ) {
	cout << str << endl;
	exit(1);
}


int main(int argc, char const *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_addr, clnt_addr;
	pid_t pid;
	struct sigaction act;
	socklen_t adr_sz;
	int str_len, state;
	char buf[BUF_SIZE];

	if (argc != 2) {
		cout << "Usage : " << argv[0] << " <port>" << endl;
		return -1;
	}

	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigaction(SIGCHLD, &act, 0);

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1) {
		error_handle("socket() errror");
	}

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

	char serv_flag[] = " from server";
	while (1) {
		adr_sz = sizeof(clnt_addr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &adr_sz);
		if (clnt_sock == -1) {
			error_handle("accept failed ...");
		}
		else {
			cout << "new connected ..." << endl;
		}

		pid = fork();
		if (pid == -1) {
			close(clnt_sock);
			continue;
		}
		if ( pid == 0) {
			close(serv_sock);
			while ( str_len = read(clnt_sock, buf, sizeof(buf)) != 0) {
				cout << buf << endl;
				strcat(buf, serv_flag);
				write(clnt_sock, buf, strlen(buf));
				memset(buf, 0, sizeof(buf));	
			}
			close(clnt_sock);
			cout << "client disconnect ..." << endl;
			return 0;
		}
		else {
			close(clnt_sock);
		}
	}
	close(serv_sock);
	return 0;
}