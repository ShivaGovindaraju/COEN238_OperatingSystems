#include <stdio.h>
#include <unistd.h>

//Additional Includes are necessary to prevent warnings
#include <stdlib.h>
#include <sys/wait.h>

int main() {
	int pi_d;
	int pid;
	pi_d = fork();
	if (pi_d == 0) {
		sleep(2);
		printf("Child Process B: pid :%d ppid:%d\n", getpid(), getppid());
	}
	if (pi_d > 0) {
		pid = fork();
		if (pid > 0) {
			/*
			 * Solution: the Parent Process must wait() until both child processes
			 * have exited before it returns, preventing the child processes
			 * from being orphaned.
			 */
			waitpid(pid, NULL, 0);
			waitpid(pi_d, NULL, 0);
			// resume normal operation
			printf("\nParent Process: pid:%d ppid :%d\n", getpid(), getppid());
		} else if (pid == 0) {
			sleep(2);
			printf("Child Process A: pid :%d ppid:%d\n", getpid(), getppid());
		}
	}
	exit(0);
}
