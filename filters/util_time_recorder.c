#include "util_time_recorder.h"

#include <stdlib.h>

struct record_data
{
  size_t size;
  size_t input_index, output_index;
  float *input, *output;
};

void increment_record_index(struct record_data *data, size_t *record_index)
{
  ++*record_index;
  if (*record_index >= data->size) *record_index = 0;
}

size_t get_past_record_index(struct record_data *data, size_t record_index, size_t steps)
{
  size_t index = record_index;
  steps %= data->size;
  if (steps > index)
  {
    steps -= index + 1;
    index = data->size - 1;
  }
  index -= steps;
  return index;
}

struct record_data *init_record_data(size_t size)
{
  struct record_data *data = malloc(sizeof(struct record_data));
  data->input_index = 0;
  data->output_index = 0;
  data->size = size;
  data->input = malloc(sizeof(float) * data->size);
  data->output = malloc(sizeof(float) * data->size);
  return data;
}

size_t get_record_size(struct record_data *data)
{
  return data->size;
}

void push_input_record(struct record_data *data, float x)
{
  data->input[data->input_index] = x;
  increment_record_index(data, &data->input_index);
}

void push_output_record(struct record_data *data, float y)
{
  data->output[data->output_index] = y;
  increment_record_index(data, &data->output_index);
}

float get_past_input(struct record_data *data, size_t steps)
{
  return data->input[get_past_record_index(data, data->input_index, steps + 1)];
}

float get_past_output(struct record_data *data, size_t steps)
{
  return data->output[get_past_record_index(data, data->output_index, steps)];
}

void free_record_data(struct record_data *data)
{
  free(data->input);
  free(data->output);
  free(data);
}

