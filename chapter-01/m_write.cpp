#include <iostream>
#include <unistd.h>
#include <string>
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
	char buf[] = "Hello world !";
	int fd = open("hello.txt", O_CREAT|O_RDWR|O_TRUNC);
	if (fd == -1) {
		error_handle(string("open() error"));
	}
	int ret = write(fd, buf, sizeof(buf));
	if (ret == -1) {
		error_handle(string("write() error"));
	}
	ret = close(fd);
	if (ret == -1) {
		error_handle(string("close() error"));
	}
	return 0;
}