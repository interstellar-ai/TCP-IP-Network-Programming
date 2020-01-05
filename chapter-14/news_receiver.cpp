#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

using namespace std;

const unsigned int BUF_SIZE = 30;

void error_handle( string str ) {
	cout << str << endl;
	exit(1);
}

int main(int argc, char* argv[]){
	int recv_sock;

	int str_len;

	struct sockaddr_in addr;
	struct ip_mreq join_adr;


	FILE *fp;
	char buf[BUF_SIZE];

	if (argc != 3) {
		cout << "Usage : " << argv[0] << " <GROUPID> <PORT>" << endl;
		return -1;
	}

	recv_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (recv_sock == -1){
		error_handle("Create socker failed ...");
	}

	memset(&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY); // multicast IP
	addr.sin_port = htons(atoi(argv[2])); // multicast port

	int ret = bind(recv_sock, (struct sockaddr *)&addr, sizeof(addr));
	if (ret == -1) {
		error_handle("bind() failed ...");
	}

	join_adr.imr_multiaddr.s_addr = inet_addr(argv[1]);
	join_adr.imr_interface.s_addr = htonl(INADDR_ANY);

	setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *)&join_adr, sizeof(join_adr)); 
	
	while(1)
	{
		str_len = recvfrom(recv_sock, buf, BUF_SIZE-1, 0, NULL, 0);
		if (str_len < 0)
			break;
		buf[BUF_SIZE] = 0;
		cout << buf << endl;
	}
	close(recv_sock);
	return 0;
}