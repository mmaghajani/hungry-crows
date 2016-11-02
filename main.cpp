using namespace std;

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
int awoker;
int feeding = 0;
sem_t restFood;
sem_t notify;
sem_t mutex;

void ready_to_eat();

void finish_eating();

void goto_sleep();

void food_ready();

void crow(void *threadid) {
    printf("Baby crow %d started", threadid);
    while (true) {
        delay(PLAYING_TIME);
        ready_to_eat(threadid);
        delay(EATING_TIME);
        printf("Baby crow %d is eating", threadid);
        finish_eating();
    }
}

void mother(void *threadid) {
    printf("Mother crow started");
    while (feeding < t) {
        goto_sleep();
        delay(PREPARING_FOOD_TIME);
        food_ready();
        delay(WAITING_TIME);
    }
    printf("Mother crow retires after serving %d feedings. Game ends!!!", feeding);
}

void food_ready() {
    feeding++;
    printf("Mother crow says \"Feeding (%d)\"", feeding);
    printf("Mother crow is awoke by baby crow %d and starts preparing food.", awoker);
    for (int i = 0; i < m; i++)
        sem_post(&restFood);
    sem_wait(&mutex);
    fillingNum = m - 1;
    sem_post(&mutex);
}

void goto_sleep() {
    printf("Mother crow takes a nap");
    sem_wait(&notify);
}

void finish_eating() {
    sem_wait(&mutex);
    if (fillingNum == 0) {
        sem_post(&restFood);
    } else {
        fillingNum--;
    }
    sem_post(&mutex);
}

void ready_to_eat(void *threadid) {
    printf("Baby crow %d is ready to eat", threadid);
    sem_wait(&restFood);
    sem_wait(&mutex);
    if (fillingNum == 0) {
        printf("Baby crow %d sees all feeding pots are empty and wakes up the mother.", threadid);
        awoker = (int) threadid;
        sem_post(&notify);
        sem_wait(&restFood);
    }
    sem_post(&mutex);
}

int main(int argc, char *argv[]) {
    cin >> n >> m >> t;
    if (n == 0) n = 10;
    if (m == 0) m = 10;
    if (t == 0) t = 10;

    int NUM_CROWS = n;
    fillingNum = m - 1;
    pthread_t crows[NUM_CROWS];
    pthread_t mother;
    sem_init(&restFood, 0, m);
    sem_init(&notify, 0, 1);
    sem_init(&mutex, 0, 1);

    printf("MAIN : There are %d baby crows, %d feeding pots , and %d feedings", n, m, t);
    printf("MAIN : Game starts!!!!!");
    int rc;
    rc = pthread_create(&mother, NULL, crow, (void *) 0);
    if (rc) {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }

    for (long t = 1; t <= NUM_CROWS; t++) {
        rc = pthread_create(&crows[t], NULL, crow, (void *) t);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    sem_destroy(&restFood);
    sem_destroy(&notify);
    sem_destroy(&mutex);
    /* Last thing that main() should do */
    pthread_exit(NULL);
}