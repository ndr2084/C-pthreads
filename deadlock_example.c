#include <pthread.h>
#include <unistd.h>

/*EXPLANATION OF CODE:
    deadlock occurs when mutexes are locked in a flipped order
*/
// MUTEX OR BARRIER DECLARATION
pthread_mutex_t mutex1, mutex2;

void* thread1_func(void* arg) {
    pthread_mutex_lock(&mutex1);
    sleep(1);  // Simulate work
    pthread_mutex_lock(&mutex2);  // can't lock mutex 2 if other function has it
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);
    return NULL;
}

void* thread2_func(void* arg) {
    pthread_mutex_lock(&mutex2);
    sleep(1);  // Simulate work
    pthread_mutex_lock(&mutex1);  // can't lock mutex 1 if other function has it
    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);

    //PTHREAD CREATION
    pthread_create(&t1, NULL, thread1_func, NULL);
    pthread_create(&t2, NULL, thread2_func, NULL);

    //PTHREAD JOIN
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    //PTHREAD MUTEX DESTRUCTION
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);
    return 0;
}
