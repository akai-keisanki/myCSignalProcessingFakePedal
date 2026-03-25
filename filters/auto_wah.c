#include "auto_wah.h"

#include <math.h>

float auto_wah(float x, float atk)
{
  static float p_y = 0.0f;
  static float env = 0.0f;

  atk /= 10.0f;

  env += (fabs(x) - env) * atk;
  env = fminf(env, 1.0f);

  float y = p_y + (x - p_y) * powf(env, 1.5f);
  p_y = y;

  return y;
}

