#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "printer.h"
#include "semaphore.h"
#include "util.h"

#define NUM_PRINTERS 3

int main() {
    Printer printers[NUM_PRINTERS];
    Semaphore semaphore;

    return 0;
}
