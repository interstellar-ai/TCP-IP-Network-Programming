#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/epoll.h>

using namespace std;

const unsigned int BUF_SIZE = 100;
const unsigned int EPOLL_SIZE = 50;

void error_handle( string str ) {
	cout << str << endl;
	exit(1);
}

int main(int argc, char const *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_addr, clnt_addr;
	fd_set reads, cpy_reads;
	socklen_t adr_sz;

	char buf[BUF_SIZE];
	char serv_falg[] = "Received from server: ";

	struct epoll_event *ep_event;
	struct epoll_event event;

	int epfd, event_cnt, str_len;

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

	epfd = epoll_create(EPOLL_SIZE);
	ep_event = (struct epoll_event *)malloc(sizeof(struct epoll_event) * EPOLL_SIZE);

	event.events = EPOLLIN;
	event.data.fd = serv_sock;

	epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);

	while (1) {
		event_cnt = epoll_wait(epfd, ep_event, EPOLL_SIZE, -1);
		if (event_cnt == -1) {
			cout << "epoll_wait() error" << endl;
			break;
		}

		for ( int i = 0; i < event_cnt; i++ ) {
			if ( ep_event[i].data.fd == serv_sock ) {
				adr_sz = sizeof(clnt_addr);
				clnt_sock = accept(serv_sock, (struct sockaddr*)&serv_addr, &adr_sz);
				if (clnt_sock == -1) {
					error_handle("accept failed ...");
				}
				else {
					cout << "new connected: " << clnt_sock <<endl;
				}
				event.events = EPOLLIN;
				event.data.fd = clnt_sock;
				epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
			}
			else {
				str_len = read(ep_event[i].data.fd, buf, BUF_SIZE);
				if ( str_len == 0) {
					epoll_ctl(epfd, EPOLL_CTL_DEL, ep_event[i].data.fd, NULL);
					close(ep_event[i].data.fd);
					cout << "close client: " << ep_event[i].data.fd << endl;
				}
				else {
					cout << "From client: " << buf << endl;
					write(ep_event[i].data.fd, buf, strlen(buf));
					memset(buf, 0, sizeof(buf));
				}
			}
		}
	}
	close(epfd);
	close(serv_sock);
	return 0;
}