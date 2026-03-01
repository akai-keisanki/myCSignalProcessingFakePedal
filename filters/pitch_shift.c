#include "pitch_shift.h"

#include <math.h>

float pitch_shift(struct record_data *grain, float x, float uni)
{
  static float read_pos = 0.0f;
  static size_t pos = -1;
  push_input_record(grain, x);
  ++pos;
  pos %= get_record_size(grain);
  float steps = (roundf(fmodf(uni, 0.1f)*10000.0f) / 10.0f)*(uni - fmodf(uni, 0.1f) > 0.0001 ? -1.0f : 1.0f);
  float dfreq = powf(2.0f, steps / 12.0f);
  read_pos += dfreq;
  read_pos = fmodf(read_pos, get_record_size(grain));
  return get_past_input(grain, pos - roundf(read_pos));
}
