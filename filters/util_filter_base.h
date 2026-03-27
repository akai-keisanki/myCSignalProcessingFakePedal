#ifndef FILTERS_UTIL_FILTER_BASE_H
#define FILTERS_UTIL_FILTER_BASE_H

#include <stddef.h>

typedef float params_t[5];

struct filter;

struct filter *init_filter(char label[], float (*apply)(struct filter *self, float x), params_t params, params_t static_vals, size_t rec_size);
void free_filter(struct filter *self);

float get_param(struct filter *self, size_t i);
void set_param(struct filter *self, size_t i, float v);
float get_static_val(struct filter *self, size_t i);
void set_static_val(struct filter *self, size_t i, float v);
struct record_data *get_record_data(struct filter *self);

float apply_filter(struct filter *self, float x);

#endif
