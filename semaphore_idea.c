#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>

#define THREAD_COUNT 5

/*EXPLANATION OF CODE:
sem_init(sem_t *sem, int pshared, unsigned int value)

-sem: this is the address of the semaphore that's been declared
-pshared: pshared := 0 indicates all threads work on the same process; all threads can be accessed by one global variable
  -a none pshared !:= 0 indicates that threads are split between more than one process; some threads belong to different global variables
-value: specifies the initial value of the semaphore
  -if value := 1, then it's a binary semaphore, which has the same behaviour as a mutex (only one thread may access the semaphore at a time)
  -if value := n, then it's an n semaphore; n threads may access it at a time)
 */

//DECLARE MUTEX AND BARRIERS AND SEMAPHORES

sem_t semaphore;

void* routine(void* args){
  //CRITICAL SECTION START
  sem_wait(&semaphore); //value of semaphore decrements from 1 to 0
  sleep(1);
  printf("Hello from the thread: %d\n", *(int*)args);
  //CRITICAL SECTION END
  sem_post(&semaphore); //value of semaphore increases from 0 to 1
  free(args);
}

int main(int argc, char* argv[]){
  //DECLARE THREADS
  pthread_t thread[THREAD_COUNT];
  //INIT MUTEXES AND SEMAPHORES
  sem_init(&semaphore, 0, 1); //parameter_1: address of sem,
                              //parameter_2: # of processes threads will work on (0 means only one process)
                              //parameter_3: # of threads that can access the semaphore. (1 gives us binary semaphore; same behaviour as mutex)

  //CREATE THREADS
  int i;
  for(i = 0; i < THREAD_COUNT; i++){
    int* a = malloc(sizeof(int));
    *a = i;
    if(pthread_create(thread + i, NULL, routine, a) != 0){
      perror("pthread_create");
    }
  }

  //JOIN THREADS
  for(i = 0; i < THREAD_COUNT; i++){
    if(pthread_join(thread[i], NULL) != 0){
      perror("pthread_join");
    }
  }

  //DESTROY MUTEXES AND SEMAPHORES
  sem_destroy(&semaphore);

  return 0;
}