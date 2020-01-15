#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

using namespace std;
int sum = 0;

void error_handle( string str ) {
	cout << str << endl;
	exit(1);
}

void* thread_sum(void *arg) {
	int start = ((int *)arg)[0];
	int end = ((int *)arg)[1];

	while (start <= end) {
		sum += start;
		start++;
	}
	return NULL;
}

int main(int argc, char const *argv[])
{
	pthread_t t_id1, t_id2;
	int rang1[] = {1, 5};
	int rang2[] = {6, 10};

	if (pthread_create(&t_id1, NULL, thread_sum, (void *)rang1) != 0) {
		error_handle("pthread_create() error");
	}

	if (pthread_create(&t_id2, NULL, thread_sum, (void *)rang2) != 0) {
		error_handle("pthread_create() error");
	}

	if (pthread_join(t_id1, NULL) != 0) {
		error_handle("thread_join() error");
	}

	if (pthread_join(t_id2, NULL) != 0) {
		error_handle("thread_join() error");
	}

	cout << "result: " << sum << endl;
	return 0;
}