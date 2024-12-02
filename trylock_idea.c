#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/*Explanation:
  -pthread_mutex_lock(&mutex) will wait indefinitely until the lock is released
  -pthread_mutex_trylock(&mutex) will try to gain access to critical area
      -if critical area access is denied, then move on to something else OR resolve gracefully
 */

void *routine(void *arg) {
    if (pthread_mutex_trylock(&mutex) == 0) {
        //pthread_mutex_lock(&mutex);
        printf("Acquired lock\n");
        pthread_mutex_unlock(&mutex);
    } else {
        printf("mutex is locked\n");
    }
}


int main(int argc, char **argv) {
    pthread_t thread[4];
    for (int i = 0; i < 4; i++) {
        if (pthread_create(&thread[i], NULL, routine, NULL) != 0) {
            perror("pthread_create");
        }
    }
    for (int i = 0; i < 4; i++) {
        if (pthread_join(thread[i], NULL) != 0) {
            perror("pthread_join");
        }
    }
    return 0;
}
