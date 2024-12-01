#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define THREAD_NUMBER 3
//CREATE BARRIER/MUTEX
pthread_barrier_t barrier;

void* routine(void* args){
  while (1) {
    printf("Waiting at the barrier...\n");
    //sleep(1);
    pthread_barrier_wait(&barrier);
    printf("Break on through to the other side!\n");
    sleep(1);
  }
}

int main(int argc, char** argv){
  pthread_t thread[THREAD_NUMBER];
  pthread_barrier_init(&barrier, NULL, THREAD_NUMBER + 1);

  //CREATE PTHREADS
  int i;
  for(i = 0; i < THREAD_NUMBER; i++) { // i < 2, then we will never break through!
    int* a = malloc(sizeof(int));
    *a = i;
    if(pthread_create(thread + i, NULL, routine, a) != 0){
      perror("pthread_create");
    }
  }
    pthread_barrier_wait(&barrier);
    //JOIN PTHREADS
    for(i = 0; i < THREAD_NUMBER; i++){
      if(pthread_join(*(thread + i), NULL) != 0){
        perror("pthread_join");
      }
    }
  exit(0);
}

