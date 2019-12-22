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
	int tcp_sock, udp_sock;
	int sock_type;
	socklen_t optlen;
	int state;

	optlen = sizeof(sock_type);
	tcp_sock = socket(PF_INET, SOCK_STREAM, 0);
	udp_sock = socket(PF_INET, SOCK_DGRAM, 0);

	cout << "SOCK_STREAM: " << SOCK_STREAM << endl;
	cout << "SOCK_DGRAM: " << SOCK_DGRAM << endl;

	state = getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
	if (state)
		error_handle("get tcp socket opt errror");
	cout << "sock type one: " << sock_type << endl;

	state = getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
	if (state)
		error_handle("get udp socket opt errror");
	cout << "sock type two: " << sock_type << endl;

	return 0;
}