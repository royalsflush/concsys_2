#ifndef PRINTER_H
#define PRINTER_H

#include <pthread.h>

struct printer;
struct semaphore;

void InitPrinter(struct printer* p, struct semaphore* s);
void PrintCurrentJob(struct printer* p);

struct printer {
    int current_job;
    pthread_mutex_t job_mutex;
    pthread_cond_t job_cond;
    struct semaphore* semaphore;
};
typedef struct printer Printer;

#endif /* PRINTER_H */
