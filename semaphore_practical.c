#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <string.h>

#define THREAD_NUM 5
#define USERS 12

//DECLARE MUTEX || BARRIER || SEMAPHORE
sem_t user;


void* routine(void* args){
  printf("waiting queue for login: %d\n", *(int*)args);
  sem_wait(&user); //CRITICAL SECTION --
  sleep(1);
  printf("Login success from user: %d\n", *(int*)args);
  sleep(rand() % 5 + 1);
  printf("Logout success from user: %d\n", *(int*)args);
  sem_post(&user); //CRITICAL SECTION ++
  free(args);
}

int main(int argc, char* argv[]){
  //DECLARE THREADS
  pthread_t thread[THREAD_NUM];
  sem_init(&user, 0, USERS);

  //INITIALIZE THREADS || MUTEXES || SEMAPHORES
  int i;
  for(i = 0; i < THREAD_NUM; i++){
    int* a = malloc(sizeof(int));
    *a = i;
    if(pthread_create(thread + i, NULL, routine, a) != 0){
      perror("pthread_create");
    }
  }



 //JOIN THREADS
  for(i = 0; i < THREAD_NUM; i++){
    if(pthread_join(thread[i], NULL) != 0){
      perror("pthread_join");
    }
  }

 //DESTROY SEMAPHORES AND MUTEXES
  sem_destroy(&user);

  return 0;
}