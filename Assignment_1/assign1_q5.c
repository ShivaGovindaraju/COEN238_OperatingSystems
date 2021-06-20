#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#include <errno.h>

int main(){
	int fds[2]; //these are the file descriptors for the pipeline
	char buff[15]; //a buffer of size 15 is a little small, but sufficient for the 10-char message we're using
	int pid1; //pid for the first forking process - to creat Child A
	int pid2 = -1; //this is initially set to -1 to keep Child A from trying to create another fork by accident.
	int count; //a variable to hold the return values of read() and write()

	char message[10] = "i love you"; //the message to be sent through the pipe

	printf("\n[Parent] Creates Pipeline.\n");
	if (pipe(fds) == -1) { //create the pipeline
		printf("Error in creation of Pipeline.\n");
		exit(2);
	}

	printf("\n[Parent] Creates Child Processes.\n"); 
	fflush(stdout); //to ensure that stdout doesn't retain anything further.

	pid1 = fork(); //fork off the first child process
	
	if (pid1 > 0) {
		//This is the Parent Process to Child A
		if ((pid2 = fork()) > 0){ //fork off the second child process
			//This is the Parent Process to Child B
			//To prevent the children from being Orphaned
			//and to keep the pipe from being closed prematurely
			//we must wait until Child A and B are both completed before
			//exiting the Parent process
			waitpid(pid2, NULL, 0);
			waitpid(pid2, NULL, 0);
			printf("\n[Parent] is done.\n");
			fflush(stdout); //clean out stdout to eliminate possible artifacts
			exit(0);
		}
	} 
	if (pid1 == 0) {
		//this is the First Child Process
		//The Writer Process
		printf("\n[Child A] is Writer to the Pipe.\n");
		close(fds[0]); //close off the Read FD
		//write the message to the pipe via the Write FD
		count = write(fds[1], message, strlen(message));
		if (count == -1) {
			printf("\n[Child A] Error occured in Writing. errno: %d\n", errno);
			exit(2);
		}
		//if all has gone well, the message has been properly written
		printf("\n[Child A] Finished Writing. Wrote %d bytes to the Pipe.\n", count);
		close(fds[1]); //need to close the Write FD for this process before exiting
		exit(0);
	} 
	if (pid2 == 0) {
		//this is the Second Child Process
		//The Reader Process
		printf("\n[Child B] is Reader to the Pipe.\n");
		close(fds[1]); //close off the Write FD
		//read the message from the pipe via the Read FD
		if ((count = read(fds[0], buff, strlen(message))) > 0) {
			printf("\n[Child B] Message: %s\n", buff);
		}
		if (count == -1) {
			printf("\n[Child B] Error in Reading. errno: %d\n", errno);
			exit(2);
		}
		//if all has gone well, the message was properly read from the pipeline
		close(fds[0]); //read to close the Read FD for this process before exiting
		printf("\n[Child B] Finished Reading.\n");
		exit(0);
	}
	return 0; //we should never get here, but this handles exit conditions in case weird things happen
}
