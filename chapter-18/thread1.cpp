#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

void error_handle( string str ) {
	cout << str << endl;
	exit(1);
}

void* thread_main(void *arg) {
	int i;
	int cnt = *((int *)arg);
	for ( int i = 0; i < cnt; i++) {
		sleep(1);
		cout << "running thread" << endl;
	}
	return NULL;
}

int main(int argc, char const *argv[])
{
	pthread_t t_id;
	int thread_param = 5;

	if (pthread_create(&t_id, NULL, thread_main, (void *)&thread_param) != 0) {
		error_handle("pthread_create() error");
	}
	sleep(10);
	cout << "end of main" << endl;
	return 0;
}