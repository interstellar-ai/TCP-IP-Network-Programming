#include <iostream>
#include <arpa/inet.h>

using namespace std;

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

	return 0;
}