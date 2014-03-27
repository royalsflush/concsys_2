#ifndef UTIL_H
#define UTIL_H

#include <pthread.h>

void SecureLock(pthread_mutex_t* mutex);
void SecureUnlock(pthread_mutex_t* mutex);

#endif /* UTIL_H */
