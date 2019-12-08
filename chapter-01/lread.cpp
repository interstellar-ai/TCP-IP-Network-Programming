#include <iostream>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
// #include <sys/stat.h>
#include <sys/types.h>

using namespace std;

void error_handle(string error_log) {
	cout << error_log << endl;
	exit(-1);
}

int main() {
	// char buf[] = "Hello world !";
	char buf[30];
	int fd = open("hello.txt", O_RDWR);
	if (fd == -1) {
		error_handle(string("open() error"));
	}
	memset(buf, 0, sizeof(buf));
	int ret = read(fd, buf, sizeof(buf));
	if (ret == -1) {
		error_handle(string("write() error"));
	}
	cout << buf << endl;
	ret = close(fd);
	if (ret == -1) {
		error_handle(string("close() error"));
	}
	return 0;
}