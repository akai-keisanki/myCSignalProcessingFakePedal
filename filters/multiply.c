#include "multiply.h"

float multiply(struct filter *self, float x)
{
  float mul = get_param(self, 0);

  mul *= 100.0f;
  return x * mul;
}

struct filter *init_filter_multiply(float mul)
{
  return init_filter("multiply", multiply, (params_t){mul}, 0);
}
