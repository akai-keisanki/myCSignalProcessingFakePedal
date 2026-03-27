#include "filter_interpreter.h"

#include <stdlib.h>
#include <stdbool.h>

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

struct filter **interpret_filter_file(const char *filter_file_name)
{
  // TODO
}

struct filter **interpret_filter_string(const char *filters_string)
{
  struct filter **filters = malloc(sizeof(struct filter *) * MAX_FILTERS_SIZE);

  size_t i = 0;
  char curr_filter;
  float v, w;
  bool pushed;

  while ((curr_filter = *filters_string++) && i < MAX_FILTERS_SIZE - 3)
  {
    pushed = false;

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
      pushed = true;
      break;

    case 'C':
      filters[i] = init_filter_clip(v);
      pushed = true;
      break;

    case 'l':
      filters[i] = init_filter_low_pass(v);
      pushed = true;
      break;

    case 'h':
      filters[i] = init_filter_high_pass(v);
      pushed = true;
      break;

    case 'm':
      filters[i] = init_filter_mid_scoop(v, w);
      pushed = true;
      break;

    case 'P':
      filters[i] = init_filter_pitch_shift(v);
      pushed = true;
      break;

    case 'S':
      filters[i] = init_filter_saturate(v);
      pushed = true;
      break;

    case 'd':
      filters[i] = init_filter_delay(v, w);
      pushed = true;
      break;

    case 'D':
      filters[i] = init_filter_drive(v, w);
      pushed = true;
      break;

    case 'c':
      filters[i] = init_filter_compress(v, w);
      pushed = true;
      break;

    case 'W':
      filters[i] = init_filter_auto_wah(v);
      pushed = true;
      break;

    case 'H':
      filters[i] = init_filter_harmonize(v, w);
      pushed = true;
      break;
    }

    if (pushed) ++i;
  }

  filters[i++] = init_filter_clip(1.0f);
  filters[i] = NULL;

  return filters;
}

void free_filter_list(struct filter **filters)
{
  for (size_t i = 0; filters[i]; ++i)
    free_filter(filters[i]);

  free(filters);
}
