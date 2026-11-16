#include <math.h>
#include <stddef.h>

#include "stats.h"

/*
 * Both functions walk the array with pointers rather than an index: the loop
 * limit is computed once, so there is no repeated compare against a changing
 * variable, and the empty/degenerate cases fall out of the guards.
 */

double compute_mean(const double arr[], int n)
{
    const double *p;
    const double *end;
    double total = 0.0;

    if (arr == NULL || n <= 0) {
        return 0.0;
    }

    end = arr + n;
    for (p = arr; p < end; ++p) {
        total += *p;
    }

    return total / (double)n;
}

double compute_stddev(const double arr[], int n, double mean)
{
    const double *p;
    const double *end;
    double squares = 0.0;

    /* One observation has no spread; n-1 would divide by zero. */
    if (arr == NULL || n < 2) {
        return 0.0;
    }

    end = arr + n;
    for (p = arr; p < end; ++p) {
        double deviation = *p - mean;
        squares += deviation * deviation;
    }

    return sqrt(squares / (double)(n - 1));
}
