#include <iostream>
#include <arpa/inet.h>

using namespace std;

int main(int argc, char const *argv[])
{
	char addr1[] = "1.2.3.4";
	char addr2[] = "1.2.3.256";

	unsigned long conv_addr = inet_addr(addr1);
	if (conv_addr == INADDR_NONE)
		cout << "Error Occored !" << endl;
	else
		cout << "conv_addr: " << conv_addr << endl;

	// 运行此行报错，原因是一个字节的能表示的最大数字是255
	conv_addr = inet_addr(addr2);
	if (conv_addr == INADDR_NONE)
		cout << "Error Occored !" << endl;
	else
		cout << "conv_addr: " << conv_addr << endl;

	return 0;
}