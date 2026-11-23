/* fire4.c - four threads of one process share the same memory.
 * All add to the same counter, a million times each, WITHOUT a lock.
 * Build: gcc -Wall -Wextra -pthread -o fire4 fire4.c
 * Run: ./fire4
 */
#include <stdio.h>
#include <pthread.h>

#define N 1000000

long counter = 0; /* shared counter */

void *work(void *arg) {
    long id = (long)arg;
    printf("thread %ld sees counter = %ld\n", id, counter);
    for (int i = 0; i < N; i++)
        counter++;
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
    printf("counter: %ld (lost updates: the race)\n", counter);
    return 0;
}
