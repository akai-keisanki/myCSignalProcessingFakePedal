#include "filter_interpreter.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

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

#define MAX_FILTERS_SIZE (size_t)0x40

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

bool fpfml_read_param(FILE *log, FILE *file, const char *exp, float *target, const char *param_name)
{
  char param_format[0x20] = "";
  strcat(param_format, param_name);
  strcat(param_format, ":");

  if (!strcmp(exp, param_format))
  {
    if (!fscanf(file, "%f", target))
      fprintf(log, "Unexpected string \"%s\" (expected %s float)\n", exp, param_name);

    return true;
  }
  return false;
}

void fpfml_param_end(FILE* log, const char *exp)
{
  if (!strcmp(exp, "]"));
  else
    fprintf(log, "Unexpected string \"%s\"\n", exp);
}

struct filter *interpret_fpfml_filter(FILE *log, FILE *file)
{
  char exp[0xFF];
  params_t v = {};

  if (fscanf(file, "%s", exp) <= 0)
  {
    fputs("File ended earlier than expected!", log);
    return NULL;
  }

  if (!strcmp(exp, "OUTPUT"))
  {
    return NULL;
  }
  else if (!strcmp(exp, "[Multiply") || !strcmp(exp, "[Multiplier"))
  {
    while (fscanf(file, "%s", exp) > 0)
    {
      if (fpfml_read_param(log, file, exp, v + 0, "multiplier"));
      else if (fpfml_read_param(log, file, exp, v + 0, "mul"));
      else if (fpfml_read_param(log, file, exp, v + 0, "coefficient"));
      else
      {
	fpfml_param_end(log, exp);
	break;
      }
    }

    return init_filter_multiply(v[0]);
  }
  else if (!strcmp(exp, "[Clip") || !strcmp(exp, "[Clipper"))
  {
    while (fscanf(file, "%s", exp) > 0)
    {
      if (fpfml_read_param(log, file, exp, v + 0, "threshold"));
      else if (fpfml_read_param(log, file, exp, v + 0, "thr"));
      else
      {
	fpfml_param_end(log, exp);
	break;
      }
    }

    return init_filter_clip(v[0]);
  }
  else if (!strcmp(exp, "[LowPass") || !strcmp(exp, "[HighCut"))
  {
    while (fscanf(file, "%s", exp) > 0)
    {
      if (fpfml_read_param(log, file, exp, v + 0, "cut"));
      else if (fpfml_read_param(log, file, exp, v + 0, "high_cut"));
      else
      {
	fpfml_param_end(log, exp);
	break;
      }
    }

    return init_filter_low_pass(v[0]);
  }
  else if (!strcmp(exp, "[HighPass") || !strcmp(exp, "[LowCut"))
  {
    while (fscanf(file, "%s", exp) > 0)
    {
      if (fpfml_read_param(log, file, exp, v + 0, "cut"));
      else if (fpfml_read_param(log, file, exp, v + 0, "low_cut"));
      else
      {
	fpfml_param_end(log, exp);
	break;
      }
    }

    return init_filter_high_pass(v[0]);
  }
  else if (!strcmp(exp, "[MidScoop") || !strcmp(exp, "[MidScooper"))
  {
    while (fscanf(file, "%s", exp) > 0)
    {
      if (fpfml_read_param(log, file, exp, v + 0, "low_cut"));
      else if (fpfml_read_param(log, file, exp, v + 0, "lcut"));
      else if (fpfml_read_param(log, file, exp, v + 1, "high_cut"));
      else if (fpfml_read_param(log, file, exp, v + 1, "hcut"));
      else
      {
	fpfml_param_end(log, exp);
	break;
      }
    }

    return init_filter_mid_scoop(v[0], v[1]);
  }
  else if (!strcmp(exp, "[PitchShift") || !strcmp(exp, "[PitchShifter"))
  {
    while (fscanf(file, "%s", exp) > 0)
    {
      if (fpfml_read_param(log, file, exp, v + 0, "unities"));
      else if (fpfml_read_param(log, file, exp, v + 0, "uni"));
      else
      {
	fpfml_param_end(log, exp);
	break;
      }
    }

    return init_filter_pitch_shift(v[0]);
  }
  else if (!strcmp(exp, "[Saturate") || !strcmp(exp, "[Saturator"))
  {
    while (fscanf(file, "%s", exp) > 0)
    {
      if (fpfml_read_param(log, file, exp, v + 0, "multiplier"));
      else if (fpfml_read_param(log, file, exp, v + 0, "mul"));
      else
      {
	fpfml_param_end(log, exp);
	break;
      }
    }

    return init_filter_saturate(v[0]);
  }
  else if (!strcmp(exp, "[Delay") || !strcmp(exp, "[Delayer"))
  {
    while (fscanf(file, "%s", exp) > 0)
    {
      if (fpfml_read_param(log, file, exp, v + 0, "delay"));
      else if (fpfml_read_param(log, file, exp, v + 0, "dly"));
      else if (fpfml_read_param(log, file, exp, v + 0, "steps"));
      else if (fpfml_read_param(log, file, exp, v + 1, "mix"));
      else
      {
	fpfml_param_end(log, exp);
	break;
      }
    }

    return init_filter_delay(v[0], v[1]);
  }
  else if (!strcmp(exp, "[Drive") || !strcmp(exp, "[Driver") || !strcmp(exp, "[OverDrive") || !strcmp(exp, "[OverDriver"))
  {
    while (fscanf(file, "%s", exp) > 0)
    {
      if (fpfml_read_param(log, file, exp, v + 0, "threshold"));
      else if (fpfml_read_param(log, file, exp, v + 0, "thr"));
      else if (fpfml_read_param(log, file, exp, v + 1, "mix"));
      else
      {
	fpfml_param_end(log, exp);
	break;
      }
    }

    return init_filter_drive(v[0], v[1]);
  }
  else if (!strcmp(exp, "[Compress") || !strcmp(exp, "[Compressor"))
  {
    while (fscanf(file, "%s", exp) > 0)
    {
      if (fpfml_read_param(log, file, exp, v + 0, "attack"));
      else if (fpfml_read_param(log, file, exp, v + 0, "atk"));
      else if (fpfml_read_param(log, file, exp, v + 1, "mix"));
      else
      {
	fpfml_param_end(log, exp);
	break;
      }
    }

    return init_filter_compress(v[0], v[1]);
  }
  else if (!strcmp(exp, "[AutoWah") || !strcmp(exp, "[Wah"))
  {
    while (fscanf(file, "%s", exp) > 0)
    {
      if (fpfml_read_param(log, file, exp, v + 0, "attack"));
      else if (fpfml_read_param(log, file, exp, v + 0, "atk"));
      else
      {
	fpfml_param_end(log, exp);
	break;
      }
    }

    return init_filter_auto_wah(v[0]);
  }
  else if (!strcmp(exp, "[Harmonize") || !strcmp(exp, "[Harmonizer"))
  {
    while (fscanf(file, "%s", exp) > 0)
    {
      if (fpfml_read_param(log, file, exp, v + 0, "unities"));
      else if (fpfml_read_param(log, file, exp, v + 0, "uni"));
      else if (fpfml_read_param(log, file, exp, v + 1, "mix"));
      else
      {
	fpfml_param_end(log, exp);
	break;
      }
    }

    return init_filter_harmonize(v[0], v[1]);
  }

  fprintf(log, "Unexpected string \"%s\"\n", exp);
  return NULL;
}

struct filter **interpret_fpfml_file(FILE *log, const char *filter_file_name)
{
  struct filter **filters = malloc(sizeof(struct filter *) * MAX_FILTERS_SIZE);

  FILE *file = fopen(filter_file_name, "r");
  if (!file)
  {
    fprintf(log, "Failed opening \"%s\".\n", filter_file_name);
    return NULL;
  }

  size_t i = 0;
  char exp[0xFF];

  while (fscanf(file, "%s", exp) > 0 && i < MAX_FILTERS_SIZE - 2)
  {
    if (!strcmp(exp, "-->"))
    {
      if (!(filters[i++] = interpret_fpfml_filter(log, file)))
      {
	filters[i - 1] = init_filter_clip(1.0f);
	break;
      }
    }
    else if (!strcmp(exp, "INPUT"));
    else
      fprintf(log, "Unexpected string \"%s\"\n", exp);
  }

  filters[i] = NULL;

  fclose(file);

  return filters;
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
      v *= 100.0f;
      filters[i++] = init_filter_multiply(v);
      break;

    case 'C':
      filters[i++] = init_filter_clip(v);
      break;

    case 'l':
      filters[i++] = init_filter_low_pass(v);
      break;

    case 'h':
      filters[i++] = init_filter_high_pass(v);
      break;

    case 'm':
      filters[i++] = init_filter_mid_scoop(v, w);
      break;

    case 'P':
      v = (roundf(fmodf(v, 0.1f)*10000.0f) / 10.0f)*(v - fmodf(v, 0.1f) > 0.0001 ? -1.0f : 1.0f);
      filters[i++] = init_filter_pitch_shift(v);
      break;

    case 'S':
      v *= 100.0f;
      filters[i++] = init_filter_saturate(v);
      break;

    case 'd':
      v *= 100000.0f;
      filters[i++] = init_filter_delay(v, w);
      break;

    case 'D':
      filters[i++] = init_filter_drive(v, w);
      break;

    case 'c':
      v /= 10.0f;
      filters[i++] = init_filter_compress(v, w);
      break;

    case 'W':
      v /= 10.0f;
      filters[i++] = init_filter_auto_wah(v);
      break;

    case 'H':
      v = (roundf(fmodf(v, 0.1f)*10000.0f) / 10.0f)*(v - fmodf(v, 0.1f) > 0.0001 ? -1.0f : 1.0f);
      filters[i++] = init_filter_harmonize(v, w);
      break;
    }
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
