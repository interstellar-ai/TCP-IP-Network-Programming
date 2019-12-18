#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

using namespace std;

void error_handle( string str ) {
	cout << str << endl;
	exit(1);
}

int main(int argc, char const *argv[])
{
	int i;
	struct hostent* host;
	if (argc != 2) {
		cout << "Usage: " << argv[0] << " <addr>" << endl;
		return -1;
	}
	host = gethostbyname(argv[1]);
	if (!host)
		error_handle("get host failed ...");

	cout << "Official name: " << host->h_name << endl;
	for (unsigned int i = 0; host->h_aliases[i]; i++ ) {
		cout << "Aliases " << i+1 << " " << host->h_aliases[i] << endl;
	}

	cout << "Adress type: ";
	if (host->h_addrtype == AF_INET)
		cout << "AF_INET" << endl;
	else
		cout << "AF_INET6" << endl;
	
	cout << "Adress :" << endl;
	for (unsigned int i = 0; host->h_addr_list[i]; i++) {
		cout << "IP addr " << i+1 << " : " << inet_ntoa(*(struct in_addr*)host->h_addr_list[i]) << endl;
	}

	return 0;
}