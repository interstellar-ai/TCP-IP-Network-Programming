#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

// struct sigaction {
// 	void (*sa_handler)(int);
// 	sigset_t sa_mask;
// 	int sa_flags;
// }

void read_childproc(int sig) {
	int status;
	pid_t pid = waitpid(-1, &status, WNOHANG);
	if (WIFEXITED(status)) {
		cout << "Remove proc id: " << pid << endl;
		cout << "Child send: " << WEXITSTATUS(status) << endl;
	}
}

int main(int argc, char const *argv[])
{
	int status;

	struct sigaction act;

	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigaction(SIGCHLD, &act, 0);

	pid_t pid = fork();

	if (pid == 0) {
		cout << "Hi, I am child process ... " << endl;
		sleep(10);
		return 12;
	}
	else {
		cout << "Child process id : " << pid << endl;
		pid = fork();
		if (pid == 0) {
			cout << "Hi, I am child process ... " << endl;
			sleep(10);
			exit(24);
		}
		else {
			cout << "Child process id : " << pid << endl;

			for (unsigned int i = 0; i < 5; i++) {
				cout << "wait ..." << endl;
				sleep(5);
			}
		}
	}
	return 0;
}