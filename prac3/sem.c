#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_PRINTERS 3
#define NO_JOB -1

typedef struct printer {
    int current_job;
    pthread_mutex_t job_mutex;
    pthread_cond_t job_cond;
} Printer;

void InitPrinter(Printer* p) {
    p->current_job = NO_JOB;
    pthread_mutex_init(&p->job_mutex, NULL);
    pthread_cond_init(&p->job_cond, NULL);
}

int main() {
    Printer printers[NUM_PRINTERS];

    return 0;
}
