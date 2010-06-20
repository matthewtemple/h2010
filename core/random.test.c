#include "h/core/random.h"
#include "h/core/tools.h"
#include "h/external/external.h"

#define H_CORE_RANDOM_TEST_ITERATIONS 30000

#define H_CORE_RANDOM_TEST_UNIFORM_PRECISION 0.01

#define H_CORE_RANDOM_TEST_POISSON_PRECISION 0.05
#define H_CORE_RANDOM_TEST_POISSON_MEAN 3

#define H_CORE_RANDOM_TEST_GAUSSIAN_PRECISION 0.05
#define H_CORE_RANDOM_TEST_GAUSSIAN_MEAN 5

int main(int argc, char *argv[])
{
  unsigned long ii = 0;
  double value = 0.0;
  double total = 0.0;
  double average = 0.0;

  srand(time(0));

  value = 0.0;
  total = 0.0;
  average = 0.0;

  for (ii = 0; ii < H_CORE_RANDOM_TEST_ITERATIONS; ii++) {
    value = h_core_random_01();
    total += value;
  }
  average = (total / H_CORE_RANDOM_TEST_ITERATIONS);
  value = fabs(average - .5);

  if (value > H_CORE_RANDOM_TEST_UNIFORM_PRECISION) {
    h_core_trace_exit("FAILED: Uniform random average: %f", average);
  }

  value = 0.0;
  total = 0.0;
  average = 0.0;

  for (ii = 0; ii < H_CORE_RANDOM_TEST_ITERATIONS; ii++) {
    value = (double) h_core_random_poisson(H_CORE_RANDOM_TEST_POISSON_MEAN);
    total += value;
  }
  average = (total / H_CORE_RANDOM_TEST_ITERATIONS);
  value = fabs(average - H_CORE_RANDOM_TEST_POISSON_MEAN);

  if (value > H_CORE_RANDOM_TEST_POISSON_PRECISION) {
    h_core_trace_exit("FAILED: Poisson random average: %f vs %f", average,
        (double) H_CORE_RANDOM_TEST_POISSON_MEAN);
  }

  value = 0.0;
  total = 0.0;
  average = 0.0;

  for (ii = 0; ii < H_CORE_RANDOM_TEST_ITERATIONS; ii++) {
    value = h_core_random_gaussian(H_CORE_RANDOM_TEST_GAUSSIAN_MEAN, 2.0);
    total += value;
  }
  average = (total / H_CORE_RANDOM_TEST_ITERATIONS);
  value = fabs(average - H_CORE_RANDOM_TEST_GAUSSIAN_MEAN);

  if (value > H_CORE_RANDOM_TEST_GAUSSIAN_PRECISION) {
    h_core_trace_exit("FAILED: Gaussian random average: %f vs %f",
        average, (double) H_CORE_RANDOM_TEST_GAUSSIAN_MEAN);
  }

  return 0;
}
