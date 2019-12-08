#include <iostream>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

void error_handle(string error_log) {
	cout << error_log << endl;
	exit(-1);
}

int main(int argc, char const *argv[])
{
	char addr[] = "1.0.0.0";
	struct sockaddr_in addr_inet;

	int ret = inet_aton(addr, &addr_inet.sin_addr);
	if (ret) {
		cout << "conversion successfully, result = " << addr_inet.sin_addr.s_addr << endl;
	}
	else
		cout << "conversion failed ..." << endl;

	char *conv_addr = inet_ntoa(addr_inet.sin_addr);
	if (*conv_addr == -1) {
		error_handle("conversion error");
	}
	char str_ip[20];
	// inet_ntoa()函数返回的是一个字符串指针，这个指针指向一个inet_ntoa函数申请的内存，
	// 应该在下次调用此函数前将字符串的内容复制到其他地方
	strcpy(str_ip, conv_addr);
	cout << "conversion str_ip: " << str_ip << endl;
	return 0;
}