#include "multiply.h"

float multiply(struct filter *self, float x)
{
  float mul = get_param(self, 0);

  return x * mul;
}

struct filter *init_filter_multiply(float mul)
{
  return init_filter("multiply", multiply, (params_t){mul}, (params_t){}, 0);
}
