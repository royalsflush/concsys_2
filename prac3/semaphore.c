#include "semaphore.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "printer.h"
#include "util.h"

void InitSemaphore(Semaphore* s) {
    s->n_elem_queue = 0;
}

void AddPrinterToEnd(Semaphore* s, Printer* p) {
    int rc;
    SecureLock(&s->queue_mutex);

    s->queue[s->n_elem_queue] = p;
    s->n_elem_queue++;

    rc = pthread_cond_signal(&s->queue_cond);
    if (rc) {
        pthread_mutex_unlock(&s->queue_mutex);
        fprintf(stderr, "Failed to wake up consumer\n");
        exit(1);
    }
    SecureUnlock(&s->queue_mutex);
}

void DispatchJob(Semaphore* s) {

}

