#include "high_pass.h"

float high_pass(float x, float cut)
{
  static float p_lp_y = 0.0f;
  float lp_y = p_lp_y + (x - p_lp_y) * cut;
  p_lp_y = lp_y;
  float y = x - lp_y;
  return y;
}

