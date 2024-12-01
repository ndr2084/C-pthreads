#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

void *routine(void *arg){
  //this works because we are passing a new address of primes + i for each call to the routine
  printf("%d ", *(int*)arg);
}

int main(int argc, char **argv){
  pthread_t threads[10];
  int i;
  for (i = 0; i < 10; i++){
    if(pthread_create(&threads[i], NULL, routine,(primes + i)) != 0){
      printf("Error creating thread\n");
    }
  }
  for (i = 0; i < 10; i++){
    if(pthread_join(threads[i], NULL) != 0){
          perror("Failed to join thread\n");
    }
  }
}