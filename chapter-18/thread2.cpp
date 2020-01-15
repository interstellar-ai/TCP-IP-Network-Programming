#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

void error_handle( string str ) {
	cout << str << endl;
	exit(1);
}

void* thread_main(void *arg) {
	int i;
	int cnt = *((int *)arg);
	char *msg = (char *)malloc(sizeof(char) * 50);
	strcpy(msg, "hello, I am thread ...");
	for ( int i = 0; i < cnt; i++) {
		sleep(1);
		cout << "running thread" << endl;
	}
	return (void *)msg;
}

int main(int argc, char const *argv[])
{
	pthread_t t_id;
	int thread_param = 5;
	void* thr_ret;

	if (pthread_create(&t_id, NULL, thread_main, (void *)&thread_param) != 0) {
		error_handle("pthread_create() error");
	}

	if (pthread_join(t_id, &thr_ret) != 0) {
		error_handle("thread_join() error");
	}
	cout << "thread return message: " << (char *)thr_ret << endl;
	free(thr_ret);
	cout << "end of main" << endl;
	return 0;
}