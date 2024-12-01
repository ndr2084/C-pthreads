#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>

/*EXPLANATION OF CODE:
- if the current mutex is taken, then pthread_mutex_trylock(<currently_used_mutex>) will be != 0
-therefore, the for loop will loop again in the routine fxn, and a thread will just use the next available mutex
-when a thread is done using its fuel, it will unlock its mutex, and break from the loop so it doesn't begin using another mutex
 */


//DECLARE MUTEX
pthread_mutex_t stove[4] = PTHREAD_MUTEX_INITIALIZER;
int stove_fuel[4] = {100, 100, 100, 100};

void* routine(void* args){
  for(int i = 0; i < 4; i++){
    	  //LOCK -- BEGIN CRITICAL
    if(pthread_mutex_trylock(stove + i) == 0){
        int fuelNeeded = (rand() % 30);
        if ((stove_fuel[i]) - fuelNeeded < 0){
          printf("No more fuel... going home\n");
        }
        else {
          stove_fuel[i] -= fuelNeeded;
          usleep(500000);
          printf("Fuel left:%d\n", stove_fuel[i]);
        }
        //UNLOCK -- END CRITICAL
        pthread_mutex_unlock(stove + i);
        break;
    } else{
      //IF (ALL MUTEXES ARE IN USE) -> WAIT 5ms
      if (i == 3){
        printf("No stove available yet; waiting...\n");
        usleep(5000000);
        i = 0; //THIS SNEAKY LIL HACK WILL MAKE THE CHEFS TRY THE FOR LOOP AGAIN
      }
    }
  }
}

int main(int argc, char** argv) {
  srand(time(NULL));
  pthread_t thread[10]; //PRETEND THAT THERE'S 10 CHEFS WHO WANNA USE A STOVE
  //INITIALIZE MUTEX
  for(int i = 0; i < 4; i++){
    pthread_mutex_init(stove + i, NULL);
  }
  //CREATE THREAD
  for(int i = 0; i < 10; i++){
    if(pthread_create(thread + i, NULL, routine, NULL) != 0){
      perror("pthread_create");
    }
  }

  //JOIN THREAD
  for (int i = 0; i < 10; i++){
    if (pthread_join(thread[i], NULL) != 0){
      perror("pthread_join\n");
    }
  }
  //DESTROY MUTEX
  for(int i = 0; i < 4; i++){
    pthread_mutex_destroy(stove + i);
  }
  return 0;
}