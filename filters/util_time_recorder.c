#include "util_time_recorder.h"

#include <stdlib.h>

size_t record_size;
size_t input_record_index, output_record_index;
float *input_record, *output_record;

void increment_record_index(size_t *record_index)
{
  ++*record_index;
  if (*record_index >= record_size) *record_index = 0;
}

size_t get_past_record_index(size_t record_index, size_t steps)
{
  size_t index = record_index;
  steps %= record_size;
  if (steps > index)
  {
    steps -= index + 1;
    index = record_size - 1;
  }
  index -= steps;
  return index;
}

void set_record_size(size_t size)
{
  input_record_index = 0;
  output_record_index = 0;
  record_size = size;
  input_record = malloc(sizeof(float *) * record_size);
  output_record = malloc(sizeof(float *) * record_size);
}

void push_input_record(float x)
{
  input_record[input_record_index] = x;
  increment_record_index(&input_record_index);
}

void push_output_record(float y)
{
  output_record[output_record_index] = y;
  increment_record_index(&output_record_index);
}

float get_past_input(size_t steps)
{
  return input_record[get_past_record_index(input_record_index, steps + 1)];
}

float get_past_output(size_t steps)
{
  return output_record[get_past_record_index(output_record_index, steps)];
}

void free_record(void)
{
  free(input_record);
  free(output_record);
}

