#include <iostream>
#include <unistd.h>
#include <signal.h>

using namespace std;

// struct sigaction {
// 	void (*sa_handler)(int);
// 	sigset_t sa_mask;
// 	int sa_flags;
// }

void timeout(int sig) {
	if (sig == SIGALRM) {
		cout << "Time out !" << endl;
	}
	alarm(2);
}

int main(int argc, char const *argv[])
{
	int i;
	struct sigaction act;

	act.sa_handler = timeout;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigaction(SIGALRM, &act, 0);

	alarm(2);

	for (unsigned int i = 0; i < 3; i++) {
		cout << "wait ..." << endl;
		sleep(100);
	}

	return 0;
}