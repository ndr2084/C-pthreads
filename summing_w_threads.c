#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

/*EXPLANATION:
-when we are sending an argument via pthread_create, we need to make sure each thread has a unique argumnet
 */

void *routine(void *arg){
  int index = *(int*)arg;
  int sum = 0;
  for(int i = 0; i < 5; i++){
    sum += primes[index + i];
  }
  *(int*)arg = sum;
  return arg;
}

int main(int argc, char **argv){
  pthread_t threads[10];

  int i;
  for (i = 0; i < 2; i++){
    int* a = malloc(sizeof(int));
    *a = i * 5;
    if(pthread_create(&threads[i], NULL, routine, a)  != 0){
      printf("Error creating thread\n");
    }
  }
  int globalSum = 0;
  int* return_value;
  for (i = 0; i < 2; i++){
    if(pthread_join(threads[i], (void**)&return_value) != 0){
          perror("Failed to join thread\n");
    }
    globalSum += *return_value;
    free(return_value);
    printf("%d ", globalSum);
  }
}