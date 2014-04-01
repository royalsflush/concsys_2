#include <pthread.h>
#include <unistd.h>

#include "printer.h"
#include "semaphore.h"
#include "util.h"

#define NO_JOB -1

void InitPrinter(Printer* p, Semaphore* s) {
    p->current_job = NO_JOB;
    pthread_mutex_init(&p->job_mutex, NULL);
    pthread_cond_init(&p->job_cond, NULL);
    p->semaphore = s;
}

void PrintCurrentJob(Printer* p) {
    SecureLock(&p->job_mutex);
    if (p->current_job > 0) {
        sleep(p->current_job);
    }
    p->current_job = NO_JOB;
    SecureUnlock(&p->job_mutex);

    AddPrinterToEnd(p->semaphore, p);
}
