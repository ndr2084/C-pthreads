#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define THREAD_NUMBER 2

/*EXPLANATION:
  [1] semaphores act like a bouncer with a hand-held clicker:
        -semaphore value corresponds to the size of the buffer
        -one semaphore to keep track of elements entering buffer
        -another semaphore to keep track of elements leaving buffer
        -the value of sem_t full and sem_t empty should always be 10 at any point in time after any wait/post block

  [2] PROPERTIES OF SEMAPHORES:
        - sem_t(sem_t *name, int pshare, unsigned int value)
            - name is the ADDRESS OF semaphore
            - pshare is the NUMBER OF PROCESSES that are shared amongst the threads. 0 implies all threads belong to a single process
            - value is NUMBER OF THREADS ALLOWED INTO CRITICAL SECTION
            - sem_wait will DECREASE the current value of value
            - sem_post will INCREASE the current value of value
 */

//DECLARE MUTEX AND SEMAPHORES
pthread_mutex_t mutex;
sem_t full;
sem_t empty;

int buffer[10];
int count = 0;

void* producer(void* args){
  //producing data
  while (1) {
    int data = rand() % 30;
    sem_wait(&empty); //DECREASE

    pthread_mutex_lock(&mutex);//CRITICAL BEGIN
    buffer[count] = data;
    count++;
    pthread_mutex_unlock(&mutex);//CRITICAL END

    sem_post(&full);
  }
}

void* consumer(void* args) {
  //consuming data
  while (1){
    sem_wait(&full);

    pthread_mutex_lock(&mutex); //CRITICAL BEGIN
    int data = buffer[count - 1];
    count--;
    pthread_mutex_unlock(&mutex);//CRITICAL END

    sem_post(&empty);

    printf("consuming data nom nom: %d\n", data);
  }
}

int main(int argc, char* argv[]){
  srand(time(NULL));
  pthread_t thread[THREAD_NUMBER];
  pthread_mutex_init(&mutex, NULL);
  sem_init(&empty, 0, 10); //when all elements are available in buffer
  sem_init(&full, 0, 0); //when no elements are left in the buffer
  int i;
  for (i = 0; i < THREAD_NUMBER; i++){
    if (i % 2 == 0) {
      if (pthread_create(thread + i, NULL, producer, NULL) != 0) {
        perror("pthread_create producer");
      }
    } else {
      if (pthread_create(thread + i, NULL, consumer, NULL) != 0){
        perror("pthread_create consumer");
      }
    }
  }

  for (i = 0; i < THREAD_NUMBER; i++){
    if (pthread_join(thread[i], NULL) != 0) {
      perror("pthread_join");
    }
  }
  pthread_mutex_destroy(&mutex);
  return 0;
}