#include <iostream>
#include <semaphore.h>
#include <pthread.h>
#include <caca_conio.h>

#define PLAYING_TIME 1000
#define EATING_TIME 1000

int fillingNum = 9;
sem_t restFood;
sem_t notify;

void ready_to_eat();

void finish_eating();

void crow(void *threadid) {
    while (true) {
        delay(PLAYING_TIME);
        ready_to_eat();
        delay(EATING_TIME);
        finish_eating();
    }
}

void finish_eating() {
    if (fillingNum == 0) {
        sem_post(&restFood);
    } else {
        fillingNum--;
    }
}

void ready_to_eat() {
    sem_wait(&restFood);
    if (fillingNum == 0) {
        sem_post(&notify);
        sem_wait(&restFood);
    }
}

int main(int argc, char *argv[]) {
    int m, n, t;
    int NUM_CROWS = n;
    int fillingNum = m - 1;
    pthread_t threads[NUM_CROWS];
    sem_init(&restFood, 0, m);
    sem_init(&notify, 0, 1);
    int rc;
    long t;
    for (t = 0; t < NUM_CROWS; t++) {
        printf("In main: creating thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, crow, (void *) t);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    sem_destroy(&restFood);
    sem_destroy(&notify);
    /* Last thing that main() should do */
    pthread_exit(NULL);
}