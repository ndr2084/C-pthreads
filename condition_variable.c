#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
/*When we are creating a global variable, and we know that both threads will be accessing the global variable,
then it only makes sense to create a mutex to control when a thread has access to the data
 */

/*EXPLANATION OF CODE:
pthread_cond_signal will SIGNAL to any sleeping thread that is waiting on a variable that needs to be updated before
it can continue with it's execution.

pthread_cond_signal WAKES UP pthread_cond_wait

pthread_cond_wait is typically used within a looping structure
 */
pthread_cond_t  mutexFuelCond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutexFuel = PTHREAD_MUTEX_INITIALIZER;
int fuel = 0;

void* fuel_filling(void* arg){
  for (int i = 0; i < 5; i++){
    pthread_mutex_lock(&mutexFuel);
    sleep(1);
    fuel += 15;
    printf("filling up glug glug: %d\n", fuel);
    pthread_mutex_unlock(&mutexFuel);
    pthread_cond_signal(&mutexFuelCond);
  }
}

void* car(void* arg){
  pthread_mutex_lock(&mutexFuel);
  while (fuel < 40) {
    printf("No fuel...Waiting for reup\n");
    sleep(1);
    pthread_cond_wait(&mutexFuelCond, &mutexFuel);
  }
  fuel -= 40;
  printf("acquired fuel, now leaving: %d\n", fuel);
  pthread_mutex_unlock(&mutexFuel);
}

int main(int argc, char** argv){
  pthread_t thread[2];

  //create section
  for (int i = 0; i < 2; i++){
    if (i == 0){
      if(pthread_create(&thread[i], NULL, fuel_filling, NULL) != 0){
        perror("pthread_create");
      }
    } else {
      if (pthread_create(&thread[i], NULL, car, NULL) != 0){
          perror("pthread_create");
      }
    }
  }
  //join section
  for (int i = 0; i < 2; i++){
    if(pthread_join(thread[i], NULL) != 0){
      perror("pthread_join");
    }
  }
  //destroy mutex section
  pthread_mutex_destroy(&mutexFuel);
  pthread_cond_destroy(&mutexFuelCond);
  return 0;
}
