#include <stddef.h>

#include "clip.h"

#include "pedal.h"

const sample_size SAMPLE_SCALING_FACTOR = 1 << (sizeof(sample_size)*8 - 1);

void apply_filters(FILE *output_wav, FILE *input_wav, char *filter_list)
{
  sample_size sample;
  float x, v;
  char *curr_filter;

  while (fread(&sample, sizeof(sample_size), 1, input_wav))
  {
    x = sample / (float)SAMPLE_SCALING_FACTOR;

    curr_filter = filter_list;
    while (*curr_filter)
    {
      switch (*curr_filter)
      {
      case 'C':

        ++curr_filter;
        v = (*curr_filter - '0') * 0.1f;
        ++curr_filter;
        v += (*curr_filter - '0') * 0.01f;

        x = clip(x, v);

        break;
      }

      ++curr_filter;
    }

    x = clip(x, 1.0f);

    sample = x * (SAMPLE_SCALING_FACTOR - 1);

    fwrite(&sample, sizeof(sample_size), 1, output_wav);
  }
}
