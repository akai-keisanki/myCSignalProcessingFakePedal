#ifndef FILTERS_UTIL_TIME_RECORDER_H
#define FILTERS_UTIL_TIME_RECORDER_H

#include <stddef.h>

void set_record_size(size_t size);

void push_input_record(float x);
void push_output_record(float y);

float get_past_input(size_t steps);
float get_past_output(size_t steps);

void free_record(void);

#endif
