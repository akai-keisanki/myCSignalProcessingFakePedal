#include "saturate.h"

#include "multiply.h"

#include <math.h>

float saturate(struct filter *self, float x)
{
  return multiply(self, tanhf(multiply(self, x)));
}

struct filter *init_filter_saturate(float mul)
{
  return init_filter("saturate", saturate, (params_t){mul}, (params_t){}, 0);
}
