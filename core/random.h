#ifndef h_core_random_h
#define h_core_random_h

double h_core_random_01();

double h_core_random_gaussian(double mean, double stddev);

unsigned long h_core_random_poisson(double mean);

unsigned long h_core_random_unsigned_long(unsigned long range);

#endif
