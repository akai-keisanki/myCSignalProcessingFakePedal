#include "pedal.h"

#include <stddef.h>
#include <stdlib.h>

const sample_size SAMPLE_SCALING_FACTOR = 1 << (sizeof(sample_size)*8 - 1);

float apply_filters(float x, struct filter **filter_list)
{
  for (size_t i = 0; filter_list[i]; ++i)
    x = apply_filter(filter_list[i], x);

  return x;
}

void pedal_in_files(FILE *output_wav, FILE *input_wav, struct filter **filter_list, const bool flush)
{
  sample_size sample;
  float x;

  while (fread(&sample, sizeof(sample_size), 1, input_wav) == 1)
  {
    x = sample / (float)SAMPLE_SCALING_FACTOR;
    x = apply_filters(x, filter_list);
    sample = x * (SAMPLE_SCALING_FACTOR - 1);

    fwrite(&sample, sizeof(sample_size), 1, output_wav);

    if (flush)
      fflush(output_wav);
  }
}
