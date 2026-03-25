#include "pedal.h"

#include <stddef.h>
#include <stdlib.h>

#include "filters/multiply.h"
#include "filters/clip.h"
#include "filters/low_pass.h"
#include "filters/high_pass.h"
#include "filters/mid_scoop.h"
#include "filters/pitch_shift.h"
#include "filters/saturate.h"
#include "filters/delay.h"
#include "filters/drive.h"
#include "filters/compress.h"
#include "filters/auto_wah.h"
#include "filters/harmonize.h"

const sample_size SAMPLE_SCALING_FACTOR = 1 << (sizeof(sample_size)*8 - 1);

struct records
{
  struct record_data *delay;
  struct record_data *pitch_shift;
  struct record_data *harmonize;
};

struct records *init_records(void)
{
  struct records *records = malloc(sizeof(struct records));

  records->delay = init_record_data(100000);
  records->pitch_shift = init_record_data(5000);
  records->harmonize = init_record_data(5000);

  return records;
}

void free_records(struct records *records)
{
  free_record_data(records->delay);
  free_record_data(records->pitch_shift);
  free_record_data(records->harmonize);
  free(records);
}

float parse_4_digit(const char **filter_p)
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

float apply_filters(struct records *records, float x, const char *filter_list)
{
  char curr_filter;
  float v, w;

  while (curr_filter = *filter_list++)
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
    case 'c':
    case 'W':
    case 'H':
      v = parse_4_digit(&filter_list);
      break;
    }

    switch (curr_filter)
    {
    case 'm':
    case 'd':
    case 'c':
    case 'H':
      w = parse_4_digit(&filter_list);
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
      x = pitch_shift(records->pitch_shift, x, v);
      break;

    case 'S':
      x = saturate(x, v);
      break;

    case 'd':
      x = delay(records->delay, x, v, w);
      break;

    case 'D':
      x = drive(x, v);
      break;

    case 'c':
      x = compress(x, v, w);
      break;

    case 'W':
      x = auto_wah(x, v);
      break;

    case 'H':
      x = harmonize(records->harmonize, x, v, w);
      break;
    }
  }

  x = clip(x, 1.0f);

  return x;
}

void pedal_in_files(FILE *output_wav, FILE *input_wav, const char *filter_list)
{
  sample_size sample;
  float x;

  struct records *records = init_records();

  while (fread(&sample, sizeof(sample_size), 1, input_wav) == 1)
  {
    x = sample / (float)SAMPLE_SCALING_FACTOR;
    x = apply_filters(records, x, filter_list);
    sample = x * (SAMPLE_SCALING_FACTOR - 1);

    fwrite(&sample, sizeof(sample_size), 1, output_wav);

    fflush(output_wav);
  }

  free_records(records);
}
