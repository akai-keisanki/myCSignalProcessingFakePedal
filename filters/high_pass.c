#include "high_pass.h"

#include "low_pass.h"

float high_pass(struct filter *self, float x)
{
  return x - low_pass(self, x);
}

struct filter *init_filter_high_pass(float cut);
{
  return init_filter("high_pass", high_pass, (params_t){cut}, (params_t){0.0f}, 0);
}
