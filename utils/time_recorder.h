#ifndef FILTERS_UTIL_TIME_RECORDER_H
#define FILTERS_UTIL_TIME_RECORDER_H

#include <stddef.h>

struct record_data;

struct record_data *init_record_data(size_t size);

size_t get_record_size(struct record_data *data);

void push_input_record(struct record_data *data, float x);
void push_output_record(struct record_data *data, float y);

float get_past_input(struct record_data *data, size_t steps);
float get_past_output(struct record_data *data, size_t steps);

void free_record_data(struct record_data *data);

#endif
