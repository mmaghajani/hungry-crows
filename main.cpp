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
int awoker;
int feeding = 0;
sem_t restFood;
sem_t isFeeding;
sem_t notify;
sem_t mutex;
sem_t console;

void ready_to_eat(int);

void finish_eating();

void goto_sleep();

void food_ready();

void *crow(void* threadid) {
    long tid;
    tid = (long)threadid;
    sem_wait(&console);
    for( int i = 0 ; i < tid ; i++)
        cout << " ";
    cout << "Baby crow " << tid << " started" << endl;
    sem_post(&console);
    while (true) {
        sleep(PLAYING_TIME);
        ready_to_eat(tid);
        sleep(EATING_TIME);
        //finish_eating();
    }
}

void *motherCrow(void *threadid) {
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
    sem_wait(&isFeeding);
    //sem_wait(&isFeeding);
    sem_wait(&console);
    cout << "Mother crow says \"Feeding (" << feeding << ")\"" << endl;
    sem_post(&console);
    sem_post(&restFood);
    for (int i = 0; i < m; i++)
        sem_post(&restFood);
    //sem_wait(&mutex);
    fillingNum = m ;
    //sem_post(&mutex);
    sem_post(&isFeeding);

}

void goto_sleep() {
    sem_wait(&console);
    cout << "Mother crow takes a nap" << endl;
    sem_post(&console);
    sem_wait(&notify);
    cout << "Mother crow is awoke by baby crow " << awoker << " and starts preparing food." << endl;
}

void finish_eating() {
//    sem_wait(&mutex);
    if (fillingNum == 0) {
        sem_post(&restFood);
    } else {
        fillingNum--;
    }
//    sem_post(&mutex);
}

void ready_to_eat(int tid) {
    sem_wait(&console);
    for( int i = 0 ; i < tid ; i++)
        cout << " ";
    cout << "Baby crow " << tid << " is ready to eat" << endl;
    sem_post(&console);
    sem_wait(&mutex);
    sem_wait(&isFeeding);
    sem_post(&isFeeding);
    if (fillingNum == 0) {
       // fillingNum++;
//        sem_wait(&restFood);
//        sem_post(&restFood);
        sem_wait(&console);
        for( int i = 0 ; i < tid ; i++)
            cout << " ";
        cout << "Baby crow " << tid << " sees all feeding pots are empty and wakes up the mother." << endl;
        sem_post(&console);
        awoker = tid;
        sem_post(&notify);
        sem_wait(&restFood);
    }
    sem_wait(&isFeeding);
    sem_post(&isFeeding);
    sem_wait(&restFood);
    sem_wait(&console);
    for( int i = 0 ; i < tid ; i++)
        cout << " ";
    cout << "Baby crow " << tid << " is eating" << endl;
    sem_post(&console);
    finish_eating();
    sem_post(&mutex);
}

int main(int argc, char *argv[]) {
    cin >> n >> m >> t;
    if (n == 0) n = 10;
    if (m == 0) m = 10;
    if (t == 0) t = 10;

    int NUM_CROWS = n;
    fillingNum = 0;
    pthread_t crows[NUM_CROWS];
    pthread_t mother;
    sem_init(&restFood, 0, 0);
    sem_init(&notify, 0, 0);
    sem_init(&mutex, 0, 1);
    sem_init(&console, 0, 1);
    sem_init(&isFeeding,0,1);

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
    sem_destroy(&isFeeding);
    /* Last thing that main() should do */
    pthread_exit(NULL);
}