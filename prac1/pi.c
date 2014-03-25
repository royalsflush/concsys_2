#include <time.h>
#include <math.h>
#include <stdio.h>

#include "monte_carlo.h"
#include "arctan.h"

#define MONTE_CARLO_IT1 1000
#define MONTE_CARLO_IT2 1000000

#define ARCTAN_TERMS1 1000
#define ARCTAN_TERMS2 1000000

clock_t t_start;

void start() {
    t_start = clock();
}

double stop() {
    return (double)(clock() - t_start)/CLOCKS_PER_SEC;
}

void parVsSeq(int arg, double (*func)(int), double (*seqFunc)(int)) {
    double par_result, par_time;
    double seq_result, seq_time;

    start();
    par_result = (*func)(arg);
    par_time = stop();

    start();
    seq_result = (*seqFunc)(arg);
    seq_time = stop();

    printf("Parallel approach - result: %.9f, time elapsed: %.9f\n",
           par_result, par_time);
    printf("Sequential approach - result: %.9f, time elapsed: %.9f\n",
           seq_result, seq_time);
}

int main(int argc, const char* argv[]) {
    printf("Actual pi: %.9f\n", acos(-1.0));
    printf("Monte carlo with %d iterations:\n", MONTE_CARLO_IT1);
    parVsSeq(MONTE_CARLO_IT1, monteCarloPi, seqMonteCarloPi);
    printf("Monte carlo with %d iterations:\n", MONTE_CARLO_IT2);
    parVsSeq(MONTE_CARLO_IT2, monteCarloPi, seqMonteCarloPi);
    printf("Arctan method (Madhava formula for pi) with %d terms:\n",
           ARCTAN_TERMS1);
    parVsSeq(ARCTAN_TERMS1, arctan1, seqArctan1);
    printf("Arctan method (Madhava formula for pi) with %d terms:\n",
           ARCTAN_TERMS2);
    parVsSeq(ARCTAN_TERMS2, arctan1, seqArctan1);
    return 0;
}
