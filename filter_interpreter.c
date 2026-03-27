#include "filter_interpreter.h"

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

#include <stdlib.h>

#define MAX_FILTERS_SIZE (size_t)20

float parse_4_digit(const char **filters_string)
{
  float v = 0.0f;
  v += (**filters_string - '0') * 0.1f;
  ++*filters_string;
  v += (**filters_string - '0') * 0.01f;
  ++*filters_string;
  v += (**filters_string - '0') * 0.001f;
  ++*filters_string;
  v += (**filters_string - '0') * 0.0001f;
  ++*filters_string;
  return v;
}

struct filter **interpret_filter_string(const char *filters_string)
{
  struct filter **filters = malloc(sizeof(struct filter *) * MAX_FILTERS_SIZE);

  size_t i = 0;
  char curr_filter;
  float v, w;

  while ((curr_filter = *filters_string++) && i < MAX_FILTERS_SIZE - 3)
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
      v = parse_4_digit(&filters_string);
      break;
    }

    switch (curr_filter)
    {
    case 'm':
    case 'd':
    case 'D':
    case 'c':
    case 'H':
      w = parse_4_digit(&filters_string);
      break;
    }

    switch (curr_filter)
    {
    case 'M':
      filters[i] = init_filter_multiply(v);
      break;

    case 'C':
      filters[i] = init_filter_clip(v);
      break;

    case 'l':
      filters[i] = init_filter_low_pass(v);
      break;

    case 'h':
      filters[i] = init_filter_high_pass(v);
      break;

    case 'm':
      filters[i] = init_filter_mid_scoop(v, w);
      break;

    case 'P':
      filters[i] = init_filter_pitch_shift(v);
      break;

    case 'S':
      filters[i] = init_filter_saturate(v);
      break;

    case 'd':
      filters[i] = init_filter_delay(v, w);
      break;

    case 'D':
      filters[i] = init_filter_drive(v, w);
      break;

    case 'c':
      filters[i] = init_filter_compress(v, w);
      break;

    case 'W':
      filters[i] = init_filter_auto_wah(v);
      break;

    case 'H':
      filters[i] = init_filter_harmonize(v, w);
      break;
    }

    ++i;
  }

  filters[++i] = init_filter_clip(1.0f);
  filters[i] = NULL;

  return filters;
}

void free_filter_list(struct filter **filters)
{
  for (size_t i = 0; filters[i]; ++i)
    free_filter(filters[i]);

  free(filters);
}
