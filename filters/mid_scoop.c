#include "mid_scoop.h"

float mid_scoop(float x, float low_cut, float high_cut)
{
  static float p_lp_y = 0.0f;
  static float p_lp2_y = 0.0f;
  float lp_y = p_lp_y + (x - p_lp_y) * high_cut;
  float lp2_y = p_lp2_y + (x - p_lp2_y) * low_cut;
  p_lp_y = lp_y;
  p_lp2_y = lp2_y;
  float hp_y = x - lp2_y;
  float y = lp_y + hp_y;
  return y;
}

