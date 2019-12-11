#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

using namespace std;

#ifndef bufsize
#define bufsize 1024
#endif

int calculate(int opnd_cnt, int m_message[], char operatr) {
	int result = m_message[0];
	for (unsigned int i = 0; i < opnd_cnt; i++)
		cout << m_message[i] << endl;
	switch (operatr) {
		case '+':
			for (unsigned int i = 1; i < opnd_cnt; i++)
				result += m_message[i];
			break;

		case '-':
			for (unsigned int i = 1; i < opnd_cnt; i++)
				result -= m_message[i];
			break;

		case '*':
			for (unsigned int i = 1; i < opnd_cnt; i++)
				result *= m_message[i];
			break;
	}
	return result;
}

void error_handle( string str ) {
	cout << str << endl;
	exit(1);
}

int main(int argc, char* argv[]){
	int serv_sock, clnt_sock;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;

	char message[bufsize];

	if (argc != 2) {
		cout << "Usage : " << argv[0] << " <port> " << endl;
		return -1;
	}

	serv_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1){
		cout << "Create server socket failed ..." << endl;
		return -1;
	}

	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	int ret = bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if (ret == -1) {
		cout << "bind failed ..." << endl;
		return -1;
	}

	ret = listen(serv_sock, 5);
	if (ret == -1) {
		cout << "listen failed ..." << endl;
		return -1;
	}

	socklen_t clnt_addr_size = sizeof(clnt_addr);
	char serv_flag[] = " from server";
	char opnd_cnt;
	int count;
	for(int i = 0; i < 5; i++) {
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
		if (clnt_sock == -1) {
			cout << "accept failed ..." << endl;
			return -1;
		}

		read(clnt_sock, &opnd_cnt, 1);
		count = int(opnd_cnt);
		cout << "opnd_cnt: " << count << endl; 
		int str_len;
		int rec_len = 0;
		memset(message, 0, sizeof(message));
		while (rec_len < count * sizeof(int) + 1) {
			str_len = read(clnt_sock, &message[rec_len], sizeof(message) - 1);
			if (str_len == -1) {
				error_handle("read() error");
			}
			rec_len += str_len;
		}
		cout << "rec_len: " << rec_len << endl;
		cout << "operatr: " << message[rec_len - 1] << endl;
		int result = calculate(count, (int*)message, message[rec_len - 1]);
		write(clnt_sock, (char *)&result, sizeof(result));
		cout << "result: " << result << endl;
		close(clnt_sock);
	}

	close(serv_sock);
	return 0;
}