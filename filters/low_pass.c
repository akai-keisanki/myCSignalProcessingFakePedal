#include "low_pass.h"

#include <stdio.h>

float low_pass(float x, float cut)
{
  static float p_y = 0.0f;
  float y = p_y + (x - p_y) * cut;
  p_y = y;
  return y;
}

