/*
 * stats.h -- descriptive statistics for the specification sample.
 *
 * The two functions are deliberately separate: the standard deviation needs
 * the mean, and passing it in avoids computing the mean twice for every item.
 */

#ifndef STEELSIM_STATS_H
#define STEELSIM_STATS_H

/* Arithmetic mean of arr[0 .. n-1]; returns 0.0 for an empty array. */
double compute_mean(const double arr[], int n);

/* Sample standard deviation of arr[0 .. n-1] (Bessel's correction, n-1 in the
   divisor), given the mean already computed by compute_mean(). */
double compute_stddev(const double arr[], int n, double mean);

#endif /* STEELSIM_STATS_H */
