#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "monte_carlo.h"

#define NUM_THREADS 6

int sum_t[NUM_THREADS];
unsigned int seed;

void* shootTarget(void* v_thread_info) {
    int* thread_info = (int*) v_thread_info;
    int num_iterations = thread_info[1];
    int thread_n = thread_info[0];
    int i;
    unsigned int thread_seed = seed + thread_n;

    for (i=0; i<num_iterations; i++) {
        double x = (double) rand_r(&thread_seed)/RAND_MAX;
        double y = (double) rand_r(&thread_seed)/RAND_MAX;
        if (x*x + y*y <= 1.) sum_t[thread_n] += 1.0;
    }

    pthread_exit(NULL);
}

double monteCarloPi(int num_iterations) {
    pthread_t threads[NUM_THREADS];
    int rc, t;
    double pi = 0.0;
    int thread_info[2];
    thread_info[1] = num_iterations;

    seed = time(NULL);
    for (t=0; t<NUM_THREADS; t++)
        sum_t[t] = 0.0;

    for (t=0; t<NUM_THREADS; t++) {
        printf("Creating thread %d\n", t);
        thread_info[0] = t;
        rc = pthread_create(&threads[t], NULL, shootTarget,
                            (void*) thread_info);
        if (rc) {
            printf("ERROR return code from pthread_create(): %d\n", rc);
            exit(-1);
        }
    }

    /* wait for threads to exit */
    for (t=0; t<NUM_THREADS; t++)
        pthread_join(threads[t], NULL);

    for (t=0; t<NUM_THREADS; t++)
        pi += (double)sum_t[t]/(NUM_THREADS*num_iterations);

    return 4*pi;
}
