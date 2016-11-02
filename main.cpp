using namespace std;

#include <iostream>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define PLAYING_TIME 1
#define EATING_TIME 1
#define PREPARING_FOOD_TIME 1
#define WAITING_TIME 1

int fillingNum = 9;
int m, n, t;
void *awoker;
int feeding = 0;
sem_t restFood;
sem_t notify;
sem_t mutex;
sem_t console;

void ready_to_eat(void *);

void finish_eating();

void goto_sleep();

void food_ready();

void* crow(void *threadid) {
    sem_wait(&console);
    cout << "Baby crow " << threadid << " started" << endl;
    sem_post(&console);
    while (true) {
        sleep(PLAYING_TIME);
        ready_to_eat(threadid);
        sleep(EATING_TIME);
        sem_wait(&console);
        cout << "Baby crow " << threadid << " is eating" << endl;
        sem_post(&console);
        finish_eating();
    }
}

void* motherCrow(void *threadid) {
    sem_wait(&console);
    cout << "Mother crow started" << endl;
    sem_post(&console);
    while (feeding < t) {
        goto_sleep();
        sleep(PREPARING_FOOD_TIME);
        food_ready();
        sleep(WAITING_TIME);
    }
    sem_wait(&console);
    cout << "Mother crow retires after serving " << feeding << " feedings. Game ends!!!" << endl;
    sem_post(&console);
    exit(0);
}

void food_ready() {
    feeding++;
    sem_wait(&console);
    cout << "Mother crow says \"Feeding ("<<feeding << ")" << endl;
    cout << "Mother crow is awoke by baby crow " << awoker << " and starts preparing food." << endl;
    sem_post(&console);
    for (int i = 0; i < m; i++)
        sem_post(&restFood);
    sem_wait(&mutex);
    fillingNum = m - 1;
    sem_post(&mutex);
}

void goto_sleep() {
    sem_wait(&console);
    cout << "Mother crow takes a nap" << endl ;
    sem_post(&console);
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
    sem_wait(&console);
    cout << "Baby crow" << threadid << "is ready to eat" << endl;
    sem_post(&console);
    sem_wait(&restFood);
    sem_wait(&mutex);
    if (fillingNum == 0) {
        sem_wait(&console);
        cout << "Baby crow" << threadid << " sees all feeding pots are empty and wakes up the mother." << endl;
        sem_post(&console);
        awoker = threadid;
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
    sem_init(&console, 0, 1);

    sem_wait(&console);
    printf("MAIN : There are %d baby crows, %d feeding pots , and %d feedings\n", n, m, t);
    printf("MAIN : Game starts!!!!!\n");
    sem_post(&console);
    int rc;
    rc = pthread_create(&mother, NULL, motherCrow, (void *) 0);
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
    sem_destroy(&console);
    /* Last thing that main() should do */
    pthread_exit(NULL);
}