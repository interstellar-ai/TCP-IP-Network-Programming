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
		sleep(15);
		return 3;
	}
	else {
		while(!waitpid(-1, &status, WNOHANG)) {
			sleep(3);
			cout << "sleep 3 sec ..." << endl;
		}

		if (WIFEXITED(status))
			cout << "child send: " << WEXITSTATUS(status) << endl;
	}
	return 0;
}