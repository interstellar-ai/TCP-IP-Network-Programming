#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

unsigned int BUF_SIZE = 30;

void error_handle( string str ) {
	cout << str << endl;
	exit(1);
}

int main(int argc, char const *argv[])
{
	fd_set reads, temps;
	int result, str_len;
	char buf[BUF_SIZE];
	struct timeval timeout;

	FD_ZERO(&reads);
	FD_SET(0, &reads);

	/*
	timeout.tv_sec = 5;
	timeout.tv_usec = 5000;
	*/

	while (1) {
		temps = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;
		result = select (1, &temps, 0, 0, &timeout);
		if (result == -1) {
			error_handle("select() error");
		}
		else if (result == 0) {
			cout << "Time out ..." << endl;
		}
		else {
			if (FD_ISSET(0, &temps))
			{
				str_len = read(0, buf, sizeof(buf));
				cout << "message: " << buf << endl;
				memset(buf, 0, sizeof(buf));
			}
		}
	}
	return 0;
}