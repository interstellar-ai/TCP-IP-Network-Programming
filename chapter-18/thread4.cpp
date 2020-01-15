#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

using namespace std;
long long sum = 0;
int NUM_THREAD = 100;

void error_handle( string str ) {
	cout << str << endl;
	exit(1);
}

void *thread_inc(void *arg) {
	for (int i = 0; i < 500000; ++i)
	{
		sum += 1;
	}
	return NULL;
}

void *thread_dec(void *arg) {
	for (int i = 0; i < 500000; ++i)
	{
		sum -= 1;
	}
	return NULL;
}

int main(int argc, char const *argv[])
{
	pthread_t thread_id[NUM_THREAD];

	cout << "sizeof long long: " << sizeof(long long) << endl;

	for (int i = 0; i < NUM_THREAD; ++i)
	{
		if (i % 2) {
			pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);
		}
		else {
			pthread_create(&(thread_id[i]), NULL, thread_dec, NULL);
		}

	}

	for (int i = 0; i < NUM_THREAD; ++i)
	{
		pthread_join(thread_id[i], NULL);
	}

	cout << "result: " << sum << endl;
	return 0;
}