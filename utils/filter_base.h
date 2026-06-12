#ifndef FILTERS_UTIL_FILTER_BASE_H
#define FILTERS_UTIL_FILTER_BASE_H

#include <stddef.h>

struct filter;

struct filter *init_filter(char label[], void *data, size_t rec_size, float (*apply)(struct filter *self, float x));
void free_filter(struct filter *self);

const char *get_filter_label(const struct filter *self);

void *get_filter_data(struct filter *self);
struct record_data *get_filter_record_data(struct filter *self);

float apply_filter(struct filter *self, float x);

#endif
