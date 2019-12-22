#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

using namespace std;

void error_handle( string str ) {
	cout << str << endl;
	exit(1);
}

int main(int argc, char const *argv[])
{
	int sock;
	int sndBuf, rcvBuf;
	socklen_t len;
	int state;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	len = sizeof(sndBuf);
	state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&sndBuf, &len);
	if (state)
		error_handle("get sndBuf socket opt errror");

	state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcvBuf, &len);
	if (state)
		error_handle("get rcvBuf socket opt errror");


	cout << "input buf size: " << rcvBuf << endl;
	cout << "output buf size: " << sndBuf << endl;

	return 0;
}