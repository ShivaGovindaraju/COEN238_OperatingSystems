#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

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
			printf("\nParent Process: pid:%d ppid :%d\n", getpid(), getppid());
		} else if (pid == 0) {
			sleep(2);
			printf("Child Process A: pid :%d ppid:%d\n", getpid(), getppid());
		}
	}
	exit(0);
}
