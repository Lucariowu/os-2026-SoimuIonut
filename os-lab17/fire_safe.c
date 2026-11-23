/* fire_safe.c - four threads adding with mutex protection.
 * Build: gcc -Wall -Wextra -pthread -o fire_safe fire_safe.c
 * Run: ./fire_safe
 */
#include <stdio.h>
#include <pthread.h>

#define N 1000000

long counter = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *work(void *arg) {
    long id = (long)arg;
    pthread_mutex_lock(&lock);
    printf("thread %ld sees counter = %ld\n", id, counter);
    pthread_mutex_unlock(&lock);

    for (int i = 0; i < N; i++) {
        pthread_mutex_lock(&lock);
        counter++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(void) {
    pthread_t th[4];
    for (long i = 0; i < 4; i++) {
        pthread_create(&th[i], NULL, work, (void *)(i + 1));
    }
    for (int i = 0; i < 4; i++) {
        pthread_join(th[i], NULL);
    }
    printf("expected: %d\n", 4 * N);
    printf("counter: %ld (safe: no race)\n", counter);
    pthread_mutex_destroy(&lock);
    return 0;
}
