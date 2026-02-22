#include <stdint.h>
#include <stdio.h>

#include "clip.c"

typedef int16_t sample_size;

void apply_filters(FILE *output_wav, FILE *input_wav, char[] filter_list)
{
  sample_size input_sample, output_sample;
  float x, v;

  while (fread(&sample, sizeof(sample_size), 1, input_wav))
  {
    x = input_sample / (float)(1 << (sizeof(sample_size)*8 - 1));
    
    switch (*filter_list)
    {
    case 'C':

      ++filter_list;
      v = (*filter_list - '0') * 0.1f;
      ++filter_list;
      v += (*filter_list - '0') * 0.01f;

      x = clip(x, am);

      break;
    }

    x = clip(x, 1.0f);

    output_sample = (int16_t)(x * (float)(1 << (sizeof(sample_size)*8 - 1) - 1));

    fwrite(&output_sample, sizeof(sample_size), 1, output_wav);

    ++filter_list;
  }
}
