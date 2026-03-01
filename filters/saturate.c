#include "saturate.h"

#include <math.h>

float saturate(float x, float mul)
{
  mul *= 100.0f;
  return tanhf(x*mul)*mul;
}
