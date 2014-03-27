#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

typedef enum { FALSE, TRUE } boolean;

/* Adapted from http://publib.boulder.ibm.com/infocenter/iseries/v5r3/index.jsp?topic=/rzahw/rzahwe17rx.htm
 Fixed to eliminate nonstandard pthread_getthreadid_np()
 Bug fixes */

#define                 NUMTHREADS     3
pthread_mutex_t         state_vars_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t          state_changed = PTHREAD_COND_INITIALIZER;
int                     dataPresent=0;
int                     sharedData=0;

/* Add ons to the original program */
/* Overflow test
#define CONSUMER_SLEEP 5
#define PRODUCER_SLEEP 1
#define BUFFER_UPPERBOUND 10
*/

/* Underflow test */
#define CONSUMER_SLEEP 1
#define PRODUCER_SLEEP 2
#define BUFFER_UPPERBOUND 10

int producer_active = 1;
int consumed_data[NUMTHREADS];

int buffer_full = 0;
pthread_mutex_t buffer_full_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  buffer_full_changed = PTHREAD_COND_INITIALIZER;

void secure_lock(pthread_mutex_t* mutex) {
    int rc = pthread_mutex_lock(mutex);
    if (rc) {
        printf("Failed to secure mutex lock");
        exit(1);
    }
}

void secure_unlock(pthread_mutex_t* mutex) {
    int rc = pthread_mutex_unlock(mutex);
    if (rc) {
        printf("Failed to unlock mutex");
        exit(1);
    }
}

void *theThread(void *threadid) {
    int   rc;

    printf("Consumer Thread %.8x: Entered\n",(int)threadid);
    secure_lock(&state_vars_mutex);

    while (dataPresent || producer_active) {
        /* The boolean dataPresent value is required for safe use of */
        /* condition variables. If no data is present we wait, other */
        /* wise we process immediately.                              */

        while (!dataPresent && producer_active) {
            printf("Consumer Thread %.8x: Wait for data to be produced\n", (int)threadid);
            rc = pthread_cond_wait(&state_changed, &state_vars_mutex);

            if (rc) {
                printf("Consumer Thread %.8x: condwait failed, rc=%d\n", (int)threadid,rc);
                pthread_mutex_unlock(&state_vars_mutex);
                exit(2);
            }
        }

        if (!producer_active && !dataPresent) {
            secure_unlock(&state_vars_mutex);
            printf("Consumer Thread %.8x: was notified that producer is "
                   "inactive\n", (int)threadid);
            return NULL;
        }

        printf("Consumer Thread %.8x: Found data or Notified, "
               "CONSUME IT while holding lock\n",
               (int)threadid);

        /* Typically an application should remove the data from being    */
        /* in the shared structure or Queue, then unlock. Processing     */
        /* of the data does not necessarily require that the lock is held */
        /* Access to shared data goes here */
        --sharedData;
        consumed_data[(int)threadid]++;

        /* We consumed the last of the data */
        if (sharedData==0) {dataPresent=0;}
        secure_unlock(&state_vars_mutex);

        /* If buffer was full, now it's not, since we just consumed data */
        secure_lock(&buffer_full_mutex);
        buffer_full = 0;

        rc = pthread_cond_signal(&buffer_full_changed); /* wake up producer */
        if (rc) {
            pthread_mutex_unlock(&buffer_full_mutex);
            printf("Producer: Failed to wake up producer, rc=%d\n", rc);
            exit(1);
        }
        secure_unlock(&buffer_full_mutex);

        /* Finally, consume the data that we got from buffer */
        sleep(CONSUMER_SLEEP);

        /* Repeat holding the lock. pthread_cond_wait releases it atomically */
        secure_lock(&state_vars_mutex);
    }
    printf("Consumer Thread %.8x: All done\n",(int)threadid);
    secure_unlock(&state_vars_mutex);
    return NULL;
}

int main(int argc, char **argv)
{
    pthread_t             thread[NUMTHREADS];
    int                   rc=0;
    int                   amountOfData=50;
    int                   i;
    clock_t start;

    printf("Enter Testcase - %s\n", argv[0]);
    start = clock();

    printf("Create/start threads\n");
    for (i=0; i <NUMTHREADS; ++i) {
        rc = pthread_create(&thread[i], NULL, theThread, (void *)i);
        consumed_data[i] = 0;
        if (rc) {
            printf("Failed to create thread %d\n", i);
            exit(3);
        }
    }

    /* The producer loop */
    while (amountOfData--) {
        printf("Producer: 'Finding' data - amount remaining: %d\n",
               amountOfData);

        secure_lock(&buffer_full_mutex);
        while (buffer_full) {
            rc = pthread_cond_wait(&buffer_full_changed, &buffer_full_mutex);

            if (rc) {
                printf("Producer: condwait failed, rc=%d\n", rc);
                pthread_mutex_unlock(&buffer_full_mutex);
                exit(2);
            }
        }
        sleep(PRODUCER_SLEEP);

        secure_lock(&state_vars_mutex);   /* Protect shared data and flag  */
        printf("Producer: Make data shared and notify consumer\n");
        ++sharedData;                          /* Add data  */

        if (sharedData == BUFFER_UPPERBOUND)
            buffer_full = 1;

        secure_unlock(&buffer_full_mutex);

        printf("sharedData: %d\n", sharedData);
        dataPresent=1;                         /* Set boolean predicate  */

        rc = pthread_cond_signal(&state_changed); /* wake up a consumer */
        if (rc) {
            pthread_mutex_unlock(&state_vars_mutex);
            printf("Producer: Failed to wake up consumer, rc=%d\n", rc);
            exit(1);
        }

        printf("Producer: Unlock shared data and flag\n");
        secure_unlock(&state_vars_mutex);
    }

    secure_lock(&state_vars_mutex);
    producer_active = 0;
    secure_unlock(&state_vars_mutex);

    rc = pthread_cond_broadcast(&state_changed);
    if (rc) {
        pthread_mutex_unlock(&state_vars_mutex);
        printf("Producer: Failed to wake up consumers, rc=%d\n", rc);
        exit(1);
    }

    printf("Wait for the threads to complete, and release their resources\n");
    for (i=0; i <NUMTHREADS; ++i) {
        rc = pthread_join(thread[i], NULL);
        if (rc) {
            printf("Failed to join thread %d\n", i);
            exit(3);
        }
    }

    printf("Amount consumed per thread\n");
    for (i=0; i <NUMTHREADS; ++i)
        printf("Thread %d: %d units\n", i, consumed_data[i]);

    printf("Clean up\n");
    rc = pthread_mutex_destroy(&state_vars_mutex);
    rc = pthread_cond_destroy(&state_changed);
    printf("Main completed\n");
    printf("CPU time: %f\n", (double)(clock()-start)/CLOCKS_PER_SEC);
    return 0;
}
