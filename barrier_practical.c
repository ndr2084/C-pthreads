#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

/*EXPLANATION OF CODE:
think of each instance of pthread_barrier_wait(&roll) (or pthread_barrier_wait(&calculate)...it doesn't matter)
as sharing a tally of all of the threads that they see.

pthread_barrier_wait(&roll) will see 8 worker threads in  roll dice function
pthread_barrier_wait(&roll) will see the 1 main  thread in the main function

together, both instances of pthread_barrier_wait(&roll) will see 8 + 1 threads.
Therefore, it can break through the barrier since 9 threads were seen between the both of them.
 */

#define THREAD_NUM 8

pthread_t z[10] = PTHREAD_INI

int dice_values[THREAD_NUM];
int status[THREAD_NUM] = { 0 };

//DECLARE MUTEX/BARRIERS HERE

pthread_barrier_t roll;
pthread_barrier_t calculator;


void* roll_dice(void* args){
  int index = *(int*)args;
  dice_values[index] = rand() % 6 + 1;
  //WAIT UNTIL ALL DICE HAVE ROLLED AND CALCULATED
  printf("Waiting...\n");
  pthread_barrier_wait(&roll);
  pthread_barrier_wait(&calculator);
  if(status[index] == 1){
    printf("(%d rolled %d) I won\n", index, dice_values[index]);
  }
  else{
    printf("(%d rolled %d) I lost\n", index, dice_values[index]);
  }
  free(args);
}

int main(int argc, char* argv[]){
  srand(time(NULL));
  pthread_t thread[THREAD_NUM];
  pthread_barrier_init(&roll, NULL, THREAD_NUM + 1); //waits for all 8 threads + main thread
  pthread_barrier_init(&calculator, NULL, THREAD_NUM + 1);
  int i;





  //PTHREAD CREATION
  for(i = 0; i < THREAD_NUM; i++){
    int* a = malloc(sizeof(int));
    *a = i;
    if(pthread_create(thread + i, NULL, roll_dice, a) != 0){
      perror("pthread_create\n");
    }
  }

  //CALCULATE THE WINNER
  pthread_barrier_wait(&roll); //MAIN THREAD WILL TRIGGER CALCULATION
  int max = 0;
  for( i = 0; i < THREAD_NUM; i++){
    if(dice_values[i] > max){
      max = dice_values[i];
    }
  }
  for( i = 0; i < THREAD_NUM; i++){
    if(dice_values[i] == max){
      status[i] = 1;
    }
    else{
      status[i] = 0;
    }
  }
  //ALL THREADS HAVE BEEN CALCULATED
  pthread_barrier_wait(&calculator); //MAIN THREAD WILL TRIGGER SYSTEM OUTPUT FROM ROLL_DICE FXN

  //PTHREAD JOIN
  for( i = 0; i < THREAD_NUM; i++){
    if(pthread_join(*(thread + i), NULL) != 0){
      perror("pthread_join\n");
    }
  }
  //DESTROY MUTEX/BARRIERS
  pthread_barrier_destroy(&roll);
  pthread_barrier_destroy(&calculator);
  return 0;
}
