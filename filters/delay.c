#include "delay.h"

float delay(struct record_data *record, float x, float dly, float mix)
{
  unsigned steps = dly * 100000.0f;
  float y = x + get_past_output(record, steps) * mix;
  push_output_record(record, y);
  return y;
}
