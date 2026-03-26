#include "util_filter_base.h"

#include "util_time_recorder.h"

#include <stdlib.h>

#define LABEL_SIZE (size_t)16

struct filter
{
  char label[LABEL_SIZE];
  float (*apply)(struct filter *self, float x);
  params_t params;
  struct record_data *rec;
};

struct filter *init_filter(char label[LABEL_SIZE], float (*apply)(struct filter *self, float x), params_t params, size_t rec_size)
{
  struct filter *self = malloc(sizeof(struct filter));

  for (size_t i = 0; i < LABEL_SIZE - 1 && label[i]; ++i)
    self->label[i] = label[i];
  self->label[LABEL_SIZE - 1] = 0;
  
  self->apply = apply;

  for (size_t i = 0; i < sizeof(params_t) / sizeof(float); ++i)
    self->params[i] = params[i];

  self->rec = (rec_size) ? init_record_data(rec_size) : NULL;

  return self;
}

void free_filter(struct filter *self)
{
  if (self->rec) free_record_data(self->rec);
  free(self);
}

float get_param(struct filter *self, size_t i)
{
  i = i % (sizeof(params_t) / sizeof(float));
  return self->params[i];
}

struct record_data *get_struct_data(struct filter *self)
{
  return self->rec;
}

float apply_filter(struct filter *self, float x)
{
  return self->apply(self, x);
}

