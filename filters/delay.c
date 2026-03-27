#include "delay.h"

#include "util_time_recorder.h"

float delay(struct filter *self, float x)
{
  unsigned dly = get_param(self, 0);
  float mix = get_param(self, 1);
  struct record_data *record = get_record_data(self);

  float y = x + get_past_output(record, dly) * mix;
  push_output_record(record, y);

  return y;
}

struct filter *init_filter_delay(float dly, float mix)
{
  return init_filter("delay", delay, (params_t){dly, mix}, (params_t){}, 100000);
}
