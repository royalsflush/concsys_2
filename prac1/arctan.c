#include "arctan.h"

#define NUM_THREADS 6

double arctan1(int num_terms) {
    return 0.;
}

double seqArctan1(int num_terms) {
    double pi_over4 = 1;
    int i;

    for (i=1; i<num_terms; i++)
        if (i%2) pi_over4 -= 1./(2*i+1);
        else pi_over4 += 1./(2*i+1);

    return 4*pi_over4;
}
