#include "util.h"

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

void SecureLock(pthread_mutex_t* mutex) {
    int rc = pthread_mutex_lock(mutex);
    if (rc) {
        fprintf(stderr, "Failed to acquire mutex lock\n");
        exit(1);
    }
}

void SecureUnlock(pthread_mutex_t* mutex) {
    int rc = pthread_mutex_unlock(mutex);
    if (rc) {
        fprintf(stderr, "Failed to unlock mutex\n");
        exit(1);
    }
}
