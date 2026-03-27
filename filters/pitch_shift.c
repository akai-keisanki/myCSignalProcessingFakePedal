#include "pitch_shift.h"

#include <math.h>

#include "util_time_recorder.h"

float pitch_shift(struct filter *self, float x)
{
  float uni = get_param(self, 0);
  float read_pos = get_static_val(self, 0);
  size_t pos = get_static_val(self, 1);
  struct record_data *grain = get_record_data(self);

  push_input_record(grain, x);
  ++pos;
  pos %= get_record_size(grain);
  float dfreq = powf(2.0f, uni / 12.0f);
  read_pos += dfreq;
  read_pos = fmodf(read_pos, get_record_size(grain) * 3);

  set_static_val(self, 0, read_pos);
  set_static_val(self, 1, pos);

  return get_past_input(grain, pos - floorf(read_pos));
}

struct filter *init_filter_pitch_shift(float uni)
{
  return init_filter("pitch_shift", pitch_shift, (params_t){uni}, (params_t){0.0f, 4999.0f}, 5000);
}
