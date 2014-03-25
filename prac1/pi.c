#include <math.h>
#include <stdio.h>

#include "monte_carlo.h"
#include "arctan.h"

#define MONTE_CARLO_IT1 1000
#define MONTE_CARLO_IT2 1000000

#define ARCTAN_TERMS1 100
#define ARCTAN_TERMS2 1000

int main(int argc, const char* argv[]) {
    printf("Actual pi: %.9f\n", acos(-1.0));
    printf("Monte carlo with %d iterations: %.9f (par), %.9f (seq)\n",
           MONTE_CARLO_IT1,
           monteCarloPi(MONTE_CARLO_IT1),
           seqMonteCarloPi(MONTE_CARLO_IT1));
    printf("Monte carlo with %d iterations: %.9f (par), %.9f (seq)\n",
           MONTE_CARLO_IT2,
           monteCarloPi(MONTE_CARLO_IT2),
           seqMonteCarloPi(MONTE_CARLO_IT2));
    printf("Arctan method (Madhava formula for pi) with %d terms: %.9f (par), "
           "%.9f (seq)\n", ARCTAN_TERMS1, 4*arctan1(ARCTAN_TERMS1),
           4*seqArctan1(ARCTAN_TERMS1));
    printf("Arctan method (Madhava formula for pi) with %d terms: %.9f (par), "
           "%.9f (seq)\n", ARCTAN_TERMS2, 4*arctan1(ARCTAN_TERMS2),
           4*seqArctan1(ARCTAN_TERMS2));

    return 0;
}
