#include "filter_interpreter.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define STR(x) #x

#include "filters/clip.h"

#include "filters/filter_includes.def"

#define MAX_FILTERS_SIZE (size_t)0x40
#define FPFDSL_MAX_STR_SIZE (size_t)0xFF

bool fpfdsl_str_safe_read(FILE *file, char *str)
{
  signed int c;
  size_t i = 0;

  while ((c = getc(file)) == ' ' || c == '\n' || c == '\t');

  while (c != ' ' && c != '\n' && c != '\t' && c != EOF && i < FPFDSL_MAX_STR_SIZE - 1)
  {
    str[i] = c;
    c = getc(file);
    ++i;
  }

  str[i] = 0;

  if (!i) return false;

  return true;
}

bool fpfdsl_read_param(FILE *log, FILE *file, const char *exp, float *target, const char *param_name)
{
  char exp2[FPFDSL_MAX_STR_SIZE];
  char param_format[FPFDSL_MAX_STR_SIZE] = "";
  strcat(param_format, param_name);
  strcat(param_format, ":");

  if (!strcmp(exp, param_format))
  {
    if (fscanf(file, "%f", target) <= 0)
    {
      if (!fpfdsl_str_safe_read(file, exp2))
      {
	fputs("File ended earlier than expected!", log);
	return NULL;
      }

      if (!fscanf(file, "%f", target) <= 0)
	fprintf(log, "Unexpected string (expected %s float)\n", param_name);

      if (!strcmp(exp2, "dB"))
	*target = powf(10.0f, *target / 20.0f);
      if (!strcmp(exp2, "\%"))
	*target /= 100.0f;
      if (!strcmp(exp2, "\%\%"))
	*target /= 10000.0f;
      else
	fprintf(log, "Unexpected string \"%s\" (expected %s float or \"dB\")\n", exp2, param_name);
    }

    return true;
  }
  return false;
}

void fpfdsl_param_end(FILE* log, const char *exp)
{
  if (!strcmp(exp, "]"));
  else
    fprintf(log, "Unexpected string \"%s\"\n", exp);
}

struct filter *interpret_fpfdsl_filter(FILE *log, FILE *file)
{
  char exp[FPFDSL_MAX_STR_SIZE];
  params_t v = {};

  if (!fpfdsl_str_safe_read(file, exp))
  {
    fputs("File ended earlier than expected!", log);
    return NULL;
  }

  if (!strcmp(exp, "OUTPUT"))
  {
    return NULL;
  }
  #define ADD_PARAM(idx, name) else if (fpfdsl_read_param(log, file, exp, v + (size_t)(idx), STR(name)));
  #define VI(idx) v[idx]
  #define ADD_VIS(...) __VA_ARGS__
  #define ADD_NAME(name) || !strcmp(exp, STR([name))
  #define ADD_FILTER(name, names, params, ids) else if (false names) { while (fpfdsl_str_safe_read(file, exp)) { if (false); params else { fpfdsl_param_end(log, exp); break; } } return init_filter_##name(ids); }
  #include "filters/available_filters.def"
  #undef ADD_FILTER
  #undef ADD_IDS
  #undef ID
  #undef ADD_PARAM

  fprintf(log, "Unexpected string \"%s\"\n", exp);
  return NULL;
}

struct filter **interpret_fpfdsl_file(FILE *log, const char *filter_file_name)
{
  struct filter **filters = malloc(sizeof(struct filter *) * MAX_FILTERS_SIZE);

  FILE *file = fopen(filter_file_name, "r");
  if (!file)
  {
    fprintf(log, "Failed opening \"%s\".\n", filter_file_name);
    return NULL;
  }

  size_t i = 0;
  char exp[FPFDSL_MAX_STR_SIZE];

  while (fpfdsl_str_safe_read(file, exp) && i < MAX_FILTERS_SIZE - 2)
  {
    if (!strcmp(exp, "-->"))
    {
      if (!(filters[i++] = interpret_fpfdsl_filter(log, file)))
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

void free_filter_list(struct filter **filters)
{
  for (size_t i = 0; filters[i]; ++i)
    free_filter(filters[i]);

  free(filters);
}
