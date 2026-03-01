#include "mid_pass.h"

float mid_pass(float x, float low_cut, float high_cut)
{
  static float p_lp_y = 0.0f;
  static float p_lp2_y = 0.0f;
  float lp_y = p_lp_y - (x - p_lp_y) * low_cut;
  float lp2_y = p_lp2_y - (x - p_lp2_y) * high_cut;
  p_lp_y = lp_y;
  p_lp2_y = lp2_y;
  float hp_y = x - lp2_y;
  float y = x - hp_y - lp_y;
  return y;
}

