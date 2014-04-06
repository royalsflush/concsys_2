#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <pthread.h>

struct semaphore;
struct printer;
struct job_gen;

void InitSemaphore(struct semaphore* s);
void AddPrinterToEnd(struct semaphore* s, struct printer* p);
void DispatchJob(struct semaphore* s);

/* A horrible way to avoid a malloc with a double pointer... */
#define NUM_PRINTERS 3

struct semaphore {
    struct printer* queue[NUM_PRINTERS];
    struct job_gen* gen_job;
    int n_elem_queue;
    pthread_mutex_t queue_mutex;
    pthread_cond_t queue_cond;
};
typedef struct semaphore Semaphore;

#endif /* SEMAPHORE_H */
