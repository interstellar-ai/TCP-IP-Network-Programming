#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>

using namespace std;

unsigned int BUF_SIZE = 100;

void error_handle( string str ) {
	cout << str << endl;
	exit(1);
}

int main(int argc, char const *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_addr, clnt_addr;
	struct timeval timeout;
	fd_set reads, cpy_reads;
	socklen_t adr_sz;

	int fd_max, str_len, fd_num;

	char buf[BUF_SIZE];
	char serv_send[BUF_SIZE];
	char serv_falg[] = "Received from server: ";
	if (argc != 2) {
		cout << "Usage : " << argv[0] << " <port>" << endl;
		return -1;
	}

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

	FD_ZERO(&reads);
	FD_SET(serv_sock, &reads);

	fd_max = serv_sock;

	while (1) {
		cpy_reads = reads;

		timeout.tv_sec = 5;
		timeout.tv_usec = 0;

		fd_num = select (fd_max + 1, &cpy_reads, 0, 0, &timeout);
		if (fd_num == -1) {
			cout << strerror(errno) << endl;
			error_handle("select() error");
		}

		if (fd_num == 0) {
			continue;
		}

		for (int i = 0; i < fd_max + 1; ++i)
		{
			if (FD_ISSET(i, &cpy_reads))
			{
				if ( i == serv_sock )
				{
					adr_sz = sizeof(clnt_sock);
					clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &adr_sz);
					if (clnt_sock == -1) {
						error_handle("accept failed ...");
					}
					else {
						cout << "new connected: " << clnt_sock <<endl;
					}

					FD_SET(clnt_sock, &reads);
					if (fd_max < clnt_sock)
						fd_max = clnt_sock;
				}
				else {
					str_len = read(i, buf, sizeof(buf));
					if (str_len == 0) {
						FD_CLR(i, &reads);
						close(i);
						cout << "close client: " << i << endl;
					}
					else {
						cout << buf << endl;
						strcat(serv_send, serv_falg);
						strcat(serv_send, buf);
						write (i, serv_send, strlen(serv_send));
					}
				}
			}
			memset(buf, 0, sizeof(buf));
			memset(serv_send, 0, sizeof(serv_send));
		}
	}
	close(serv_sock);
	return 0;
}