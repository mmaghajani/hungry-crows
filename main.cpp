#include <iostream>
#include <semaphore.h>
#include <pthread.h>
#include <caca_conio.h>

#define PLAYING_TIME 1000
#define EATING_TIME 1000
#define PREPARING_FOOD_TIME 1000
#define WAITING_TIME 1000

int fillingNum = 9;
int m, n, t;
sem_t restFood;
sem_t notify;

void ready_to_eat();

void finish_eating();

void goto_sleep();

void food_ready();

void crow(void *threadid) {
    while (true) {
        delay(PLAYING_TIME);
        ready_to_eat();
        delay(EATING_TIME);
        finish_eating();
    }
}

void mother(void *threadid) {
    while (t > 0) {
        goto_sleep();
        delay(PREPARING_FOOD_TIME);
        food_ready();
        delay(WAITING_TIME);
    }
}

void food_ready() {
    for (int i = 0; i < m; i++)
        sem_post(&restFood);
    fillingNum = m - 1;
}

void goto_sleep() {
    sem_wait(&notify);
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

    int NUM_CROWS = n;
    int fillingNum = m - 1;
    pthread_t crows[NUM_CROWS];
    pthread_t mother;
    sem_init(&restFood, 0, m);
    sem_init(&notify, 0, 1);

    int rc;
    rc = pthread_create(&mother, NULL, crow, (void *) 0);
    if (rc) {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }

    for (long t = 1; t <= NUM_CROWS; t++) {
        printf("In main: creating thread %ld\n", t);
        rc = pthread_create(&crows[t], NULL, crow, (void *) t);
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