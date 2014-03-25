#ifndef ARCTAN_H
#define ARCTAN_H

/*
    Parallel approach for calculating arctan of 1, num_terms is the number of
    terms used in the infinite series.
*/

double arctan1(int num_terms);
/*
    Sequential approach for calculating arctan of 1, num_terms is the number of
    terms used in the infinite series.
*/

double seqArctan1(int num_terms);

#endif /* ARCTAN_H */
