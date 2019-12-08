#include <iostream>
#include <arpa/inet.h>

using namespace std;

int main(int argc, char const *argv[])
{
	unsigned short host_port = 0x1234;
	unsigned short net_port;
	unsigned long host_addr = 0x12346578;
	unsigned long net_addr;

	net_port = htons(host_port);
	net_addr = htonl(host_addr);

	cout << "host_port: " << hex << host_port << endl;
	cout << "net_port: " << hex << net_port << endl;
	cout << "host_addr: " << hex << host_addr << endl;
	cout << "net_addr: " << hex << net_addr << endl;

	return 0;
}