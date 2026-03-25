#include "compress.h"

#include <math.h>

float compress(float x, float atk, float mix)
{
  static float env = 0.0f;

  atk /= 10.0f;

  env += (fabs(x) - env) * atk;

  float gain = powf(tanh(0.1f / (env + 0.05f)), 2.0f) * 4.0f;

  return x * gain * mix + x * (1.0f - mix);
}

