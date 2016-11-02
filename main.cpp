#include <iostream>
#include <semaphore.h>
#include <pthread.h>
#include <caca_conio.h>

#define PLAYING_TIME 1000
#define EATING_TIME 1000

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

}

void ready_to_eat() {
    wait(s);
}

int main(int argc, char *argv[]) {
    int NUM_CROWS = 5;
    int fillingNum = 10;
    pthread_t threads[NUM_CROWS];
    int rc;
    long t;
    for (t = 0; t < NUM_CROWS; t++) {
        printf("In main: creating thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, PrintHello, (void *) t);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    /* Last thing that main() should do */
    pthread_exit(NULL);
}