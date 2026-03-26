#ifndef FILTERS_UTIL_FILTER_BASE_H
#define FILTERS_UTIL_FILTER_BASE_H

#include <stddef.h>

typedef float params_t[5];

struct filter;

struct filter *init_filter(char label[], float (*apply)(struct filter *self, float x), params_t params, size_t rec_size);
void free_filter(struct filter *self);

float get_param(struct filter *self, size_t i);
struct record_data *get_struct_data(struct filter *self);

float apply_filter(struct filter *self, float x);

#endif
