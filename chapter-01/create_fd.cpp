#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

int main() {
	int fd1, fd2, fd3;
	fd1 = socket(PF_INET, SOCK_STREAM, 0);
	fd2 = open("test.txt", O_CREAT|O_RDWR|O_TRUNC);
	fd3 = socket(PF_INET, SOCK_STREAM, 0);

	cout << "file descriptor 1: " << fd1 << endl;
	cout << "file descriptor 2: " << fd2 << endl;
	cout << "file descriptor 3: " << fd3 << endl;

	close(fd1);
	close(fd2);
	close(fd3);
}