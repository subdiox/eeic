#include <stdio.h>
#include <stdlib.h>
#include "optimize.h"
#include "func2.h"

int main(const int argc, const char **argv)
{
  const double alpha = (argc == 2) ? atof(argv[1]) : 0.01;

  int i;
  const int dim = f_dimension();

  double *x = malloc(dim * sizeof(double));
  for (i = 0; i < dim; i++) {
    x[i] = 0;
  }

  printf("alpha = %f\n", alpha);

  const int iter = optimize(alpha, dim, x, f_gradient, f_value);

  printf("number of iterations = %d\n", iter);
  printf("a: %f, b: %f\n", x[0], x[1]);

  free(x);

  return 0;
}
