#include <stddef.h>

#include "filters/multiply.h"
#include "filters/clip.h"
#include "filters/low_pass.h"
#include "filters/high_pass.h"
#include "filters/mid_scoop.h"
#include "filters/pitch_shift.h"
#include "filters/saturate.h"
#include "filters/delay.h"
#include "filters/drive.h"

#include "pedal.h"

const sample_size SAMPLE_SCALING_FACTOR = 1 << (sizeof(sample_size)*8 - 1);

float parse_4_digit(char **filter_p)
{
  float v = 0.0f;
  v += (**filter_p - '0') * 0.1f;
  ++*filter_p;
  v += (**filter_p - '0') * 0.01f;
  ++*filter_p;
  v += (**filter_p - '0') * 0.001f;
  ++*filter_p;
  v += (**filter_p - '0') * 0.0001f;
  ++*filter_p;
  return v;
}

void apply_filters(FILE *output_wav, FILE *input_wav, char *filter_list)
{
  sample_size sample;
  float x, v, w;
  char *filter_p;
  char curr_filter;

  struct record_data *delay_record = init_record_data(100000);

  while (fread(&sample, sizeof(sample_size), 1, input_wav) == 1)
  {
    x = sample / (float)SAMPLE_SCALING_FACTOR;

    filter_p = filter_list;
    while (curr_filter = *filter_p++)
    {
      switch (curr_filter)
      {
      case 'M':
      case 'C':
      case 'l':
      case 'h':
      case 'm':
      case 'P':
      case 'S':
      case 'd':
      case 'D':
        v = parse_4_digit(&filter_p);
        break;
      }

      switch (curr_filter)
      {
      case 'm':
      case 'd':
        w = parse_4_digit(&filter_p);
        break;
      }

      switch (curr_filter)
      {
      case 'M':
        x = multiply(x, v);
        break;

      case 'C':
        x = clip(x, v);
        break;

      case 'l':
        x = low_pass(x, v);
        break;

      case 'h':
        x = high_pass(x, v);
        break;

      case 'm':
        x = mid_scoop(x, v, w);
        break;

      case 'P':
        x = pitch_shift(x, v);
        break;

      case 'S':
        x = saturate(x, v);
        break;

      case 'd':
        x = delay(delay_record, x, v, w);
        break;

      case 'D':
        x = drive(x, v);
        break;
      }
    }

    x = clip(x, 1.0f);

    sample = x * (SAMPLE_SCALING_FACTOR - 1);

    fwrite(&sample, sizeof(sample_size), 1, output_wav);
  }

  free_record_data(delay_record);
}
