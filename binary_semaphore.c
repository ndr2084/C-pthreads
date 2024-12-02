#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>
#include<pthread.h>

/*EXPLANATION OF CODE:
  -binary sem's can be a more convenient way of ensuring a task is completed
  -it can give the benefits of both a barrier and a mutex
  -the thread will not evaporate until fuel has been added, since sem_wait cannot be less than 0
    -it must collect a coin from sem_post before it terminates
    -to get this coin, the thread must visit get_fuel function
 */

#define THREAD_NUMBER 2
int* fuel;
sem_t myfuel;

void* get_fuel(void* args){
  sem_post(&myfuel);
  *fuel += 50;
}

int main(int argc, char* argv[]){
  pthread_t thread[THREAD_NUMBER];
  sem_init(&myfuel, 0, 0);
  fuel = malloc(sizeof(int));
  *fuel = 50;
  int i;
  for(i = 0; i < THREAD_NUMBER; i++){
    if(pthread_create(thread + i, NULL, get_fuel, NULL) != 0){
      perror("pthread_create");
    }
  }
  printf("waiting\n");
  sem_wait(&myfuel);
  printf("task done! fuel: %d\n", *fuel);
  free(fuel);

  for (i = 0; i < THREAD_NUMBER; i++) {
    if (pthread_join(thread[i], NULL) != 0){
      perror("pthread_join");
    }
  }
  sem_destroy(&myfuel);
  exit(0);
}

