#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main(int argc, char const *argv[])
{
	int status;
	pid_t pid = fork();

	if (pid == 0) {
		sleep(5);
		return 3;
	}
	else {
		cout << "child pid : " << pid << endl;

		pid = fork();
		if (pid == 0) {
			sleep(5);
			exit(7);
		}
		else {
			cout << "child pid : " << pid << endl;
			wait(&status);
			if (WIFEXITED(status))
				cout << "child send one: " << WEXITSTATUS(status) << endl;

			wait(&status);
			if (WIFEXITED(status))
				cout << "child send two: " << WEXITSTATUS(status) << endl;

			sleep(10);
		}
	}
	return 0;
}