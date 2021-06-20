/*
 * @author: Shiva K Govindaraju
 * Winter 2020
 * COEN 283, Assignment 2
 */
#include <stdio.h>
#include <pthread.h>

//adding some more Includes because the compiler Warnings are annoying
#include <stdlib.h>
#include <string.h>

char USAGE[] = "naive_lock n_threads\n"
  "USAGE: run n threads with a naive lock\n";

//This is no longer useful
/*
int lock = 0; //0 for unlocked, 1 for locked
*/

//Replace the int lock with a mutex lock
pthread_mutex_t lock;

int shared = 0; //shared variable

//Replacing this function with a corrected version
/*
void * incrementer(void * args){ 
    int i; 
    for(i=0;i<100;i++){ 
    //check lock
    while(lock > 0);  //spin until unlocked
    lock = 1; //set lock
    shared++; //increment
    lock = 0; //unlock
  }
  return NULL;
}
*/

//This is the correcter function with a mutex-lock to protect the critical section
void * incrementer(void * args) {
	int i;
	for (i = 0; i < 100; i++) {
		pthread_mutex_lock(&lock); 	//Acquire the lock
		shared++;			//Access the shared resource in the critical section
		pthread_mutex_unlock(&lock);	//Release the lock
	}
	return NULL;
}

int main(int argc, char * argv[]){
  pthread_t * threads;
  int n,i;

  if(argc < 2){
    fprintf(stderr, "ERROR: Invalid number of threads\n");
    exit(1);
  }

  //convert argv[1] to a long
  if((n = atol(argv[1])) == 0){
    fprintf(stderr, "ERROR: Invalid number of threads\n");
    exit(1);
  }

  //Must Initialize the Mutex Lock
  //Without this intialization, the lock will not be properly
  //set-up and cannot guarantee mutual-exclusion
  //Therefore, the mutex must be initialized before we can use it
  if (pthread_mutex_init(&lock, NULL) != 0){
	  fprintf(stderr, "Error: Unable to Initialize Mutex.\n");
	  exit(1);
  }

  //allocate array of pthread_t identifiers
  threads = calloc(n,sizeof(pthread_t));

  //create n threads
  for(i=0;i<n;i++){
    pthread_create(&threads[i], NULL, incrementer, NULL);
  }

  //join all threads
  for(i=0;i<n;i++){
    pthread_join(threads[i], NULL);
  }

  //print shared value and result
  printf("Shared: %d\n",shared);
  printf("Expect: %d\n",n*100);

  //To Prevent Data Leakage, I had to add this free() call
  free(threads);

  return 0;
}
