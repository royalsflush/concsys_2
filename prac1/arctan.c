#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "arctan.h"

#define NUM_THREADS 6

double sum_t[NUM_THREADS];
int num_iterations;

void* sumTerms(void* v_thread_info) {
    int thread_n = (int)v_thread_info;
    int i;

    int start = thread_n*num_iterations+1;
    int end = (thread_n+1)*num_iterations+1;

    printf("Thread %d, num_iterations %d\n", thread_n, num_iterations);
    printf("%d %d\n", start, end);

    for (i=start; i<end; i++)
        if (i%2) sum_t[thread_n] -= 1./(2*i+1);
        else sum_t[thread_n] += 1./(2*i+1);

    pthread_exit(NULL);
}

double arctan1(int num_terms) {
    pthread_t threads[NUM_THREADS];
    double pi_over4 = 1;
    int t, rc;
    num_iterations = num_terms;

    for (t=0; t<NUM_THREADS; t++)
        sum_t[t] = 0.0;

    for (t=0; t<NUM_THREADS; t++) {
        rc = pthread_create(&threads[t], NULL, sumTerms, (void*)t);
        if (rc) {
            printf("ERROR return code from pthread_create(): %d\n", rc);
            exit(-1);
        }
    }

    for (t=0; t<NUM_THREADS; t++)
        pthread_join(threads[t], NULL);

    for (t=0; t<NUM_THREADS; t++)
        pi_over4 += (double)sum_t[t];

    return 4*pi_over4;
}

double seqArctan1(int num_terms) {
    double pi_over4 = 1;
    int i;

    for (i=1; i<num_terms*NUM_THREADS; i++)
        if (i%2) pi_over4 -= 1./(2*i+1);
        else pi_over4 += 1./(2*i+1);

    return 4*pi_over4;
}
