#include "semaphore.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "printer.h"
#include "util.h"

void InitSemaphore(Semaphore* s, Printer p[]) {
    int i;
    for (i=0; i<NUM_PRINTERS; i++)
        s->queue[i] = &p[i];
    s->n_elem_queue = NUM_PRINTERS;

    pthread_mutex_init(&s->queue_mutex, NULL);
    pthread_cond_init(&s->queue_cond, NULL);
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
    int rc, i;
    printf("Start dispatch job process...\n");

    SecureLock(&s->queue_mutex);
    if (!s->n_elem_queue) {
        printf("Waiting for an available printer\n");
        rc = pthread_cond_wait(&s->queue_cond, &s->queue_mutex);
        if (rc) {
            fprintf(stderr, "Failed cond wait for semaphore");
            exit(1);
        }
    }

    printf("Dispatching job to printer %d\n", );
    Printer* job_printer = s->queue[0];
    for (i=1; i<NUM_PRINTERS; i++)
        s->queue[i-1] = s->queue[i];

    ReceivePrintJob(job_printer, GetFirstPrintJob(s->gen_job));
    SecureUnlock(&s->queue_mutex);
}

void* RunSemaphore(void* v_sem) {
    Semaphore* s = (Semaphore*) v_sem;
    printf("Starting semaphore...\n");

    SecureLock(&s->queue_mutex);
    while (Active(s->gen_job)) {
        if (!Active(s->gen_job) || EmptyJobQueue(s->gen_job)) {
            printf("Waiting for job generator to do something\n");
            WaitForJobOrTurnOff(s->gen_job, s);
        }

        if (EmptyJobQueue(s->gen_job) && !Active(s->gen_job)) {
            printf("No jobs left and generator is off, semaphore is turning "
                   "off too\n");
            SecureUnlock(&s->queue_mutex);
            return NULL;
        }

        DispatchJob(s);
    }
}
