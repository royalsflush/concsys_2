#ifndef MONTE_CARLO_H
#define MONTE_CARLO_H

/*
    Given a num_iterations, returns the value of pi calculated. There's
    something wrong with it, can't seem to make it converge.
    Parallel approach to the calculation.
*/

double monteCarloPi(int num_iterations);

/*
    Given a num_iterations, returns the value of pi calculated.
    Sequential approach to the calculation.
*/

double seqMonteCarloPi(int num_iterations);

#endif /* MONTE_CARLO_H */
