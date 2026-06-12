#include "util_filter_base.h"

#include "util_time_recorder.h"

#include <stdlib.h>

#define LABEL_SIZE (size_t)16

struct filter
{
  char label[LABEL_SIZE];
  void *data;
  struct record_data *rec;
  float (*apply)(struct filter *self, float x);
};

struct filter *init_filter(char label[], void *data, size_t rec_size, float (*apply)(struct filter *self, float x))
{
  struct filter *self = malloc(sizeof(struct filter));

  for (size_t i = 0; i < LABEL_SIZE - 1 && label[i]; ++i)
    self->label[i] = label[i];
  self->label[LABEL_SIZE - 1] = 0;
  
  self->rec = (rec_size) ? init_record_data(rec_size) : NULL;

  self->data = data;

  self->apply = apply;

  return self;
}

void free_filter(struct filter *self)
{
  if (self->data) free(self->data);
  if (self->rec) free_record_data(self->rec);
  free(self);
}

const char *get_filter_label(const struct filter *self)
{
  return self->label;
}

void *get_filter_data(struct filter *self)
{
  return self->data;
}

struct record_data *get_filter_record_data(struct filter *self)
{
  return self->rec;
}

float apply_filter(struct filter *self, const float x)
{
  push_input_record(self->rec, x);

  float y = self->apply(self, x);

  push_output_record(self->rec, y);

  return y;
}

