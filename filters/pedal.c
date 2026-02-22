#include "pedal.h"

#include "clip.h"

void apply_filters(FILE *output_wav, FILE *input_wav, char *filter_list)
{
  sample_size sample;
  float x, v;

  while (fread(&sample, sizeof(sample_size), 1, input_wav))
  {
    x = sample / (float)(1 << (sizeof(sample_size)*8 - 1));
    
    switch (*filter_list)
    {
    case 'C':

      ++filter_list;
      v = (*filter_list - '0') * 0.1f;
      ++filter_list;
      v += (*filter_list - '0') * 0.01f;

      x = clip(x, v);

      break;
    }

    x = clip(x, 1.0f);

    sample = (int16_t)(x * (float)(1 << (sizeof(sample_size)*8 - 1) - 1));

    fwrite(&sample, sizeof(sample_size), 1, output_wav);

    ++filter_list;
  }
}
