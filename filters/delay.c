#include "delay.h"

#include "util_time_recorder.h"

#include <stddef.h>
#include <math.h>

float delay(struct filter *self, float x)
{
  size_t dly = get_param(self, 0);
  float fdb = get_param(self, 1);
  float mix = get_param(self, 2);
  struct record_data *record = get_record_data(self);

  float y = x + get_past_output(record, dly) * fdb;
  push_output_record(record, y);

  return (y - x) * mix + x;
}

struct filter *init_filter_delay(float dly, float fdb, float mix)
{
  return init_filter("delay", delay, (params_t){dly, fdb, mix}, (params_t){}, (size_t)ceil(dly) + 0x10);
}
