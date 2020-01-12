#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

void error_handle( string str ) {
	cout << str << endl;
	exit(1);
}

int main(int argc, char const *argv[])
{
	FILE *fp;
	int fd = open("data.txt", O_WRONLY | O_CREAT | O_TRUNC);
	if (fd == -1) {
		error_handle("file open error");
	}

	fp = fdopen(fd, "w");
	fputs("network c programming\n", fp);
	fclose(fp);
	return 0;
}