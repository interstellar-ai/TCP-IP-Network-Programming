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
	int send_sock;

	struct sockaddr_in broad_addr;
	int so_brd= 1;

	FILE *fp;
	char buf[BUF_SIZE];

	if (argc != 3) {
		cout << "Usage : " << argv[0] << " <IP> <port>" << endl;
		return -1;
	}

	send_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (send_sock == -1){
		error_handle("Create socker failed ...");
	}

	memset(&broad_addr, 0, sizeof(broad_addr));

	broad_addr.sin_family = AF_INET;
	broad_addr.sin_addr.s_addr = inet_addr(argv[1]); // multicast IP
	broad_addr.sin_port = htons(atoi(argv[1])); // multicast port

	setsockopt(send_sock, SOL_SOCKET, SO_BROADCAST, (void *)&so_brd, sizeof(so_brd)); 

	if ((fp = fopen("news.txt", "r")) == NULL)
		error_handle("open file failed ...");

	while(!feof(fp)) // broadcasting ...
	{
		fgets(buf, BUF_SIZE, fp);
		sendto(send_sock, buf, strlen(buf), 0, (struct sockaddr*)&broad_addr, sizeof(broad_addr));
		cout << buf << endl;
		sleep(2);
	}
	fclose(fp);
	close(send_sock);
	return 0;
}