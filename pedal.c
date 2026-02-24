#include <stddef.h>

#include "filters/util_time_recorder.h"

#include "filters/multiply.h"
#include "filters/clip.h"
#include "filters/equalise.h"
#include "filters/pitch_shift.h"
#include "filters/drive.h"

#include "pedal.h"

const sample_size SAMPLE_SCALING_FACTOR = 1 << (sizeof(sample_size)*8 - 1);

void apply_filters(FILE *output_wav, FILE *input_wav, char *filter_list)
{
  sample_size sample;
  float x, v, w;
  char *filter_p;
  char curr_filter;

  set_record_size(10000);

  while (fread(&sample, sizeof(sample_size), 1, input_wav))
  {
    x = sample / (float)SAMPLE_SCALING_FACTOR;

    push_input_record(x);

    filter_p = filter_list;
    while (curr_filter = *filter_p++)
    {
      switch (curr_filter)
      {
      case 'M':
      case 'C':
      case 'E':
      case 'P':
      case 'D':
        v = (*filter_p - '0') * 1.0f;
        ++filter_p;
        v += (*filter_p - '0') * 0.1f;
        ++filter_p;
        v += (*filter_p - '0') * 0.01f;
        ++filter_p;
        v += (*filter_p - '0') * 0.02f;
        ++filter_p;

        break;
      }

      switch (curr_filter)
      {
      case 'E':
        w = (*filter_p - '0') * 1.0f;
        ++filter_p;
        w += (*filter_p - '0') * 0.1f;
        ++filter_p;
        w += (*filter_p - '0') * 0.01f;
        ++filter_p;
        w += (*filter_p - '0') * 0.02f;
        ++filter_p;

        break;
      }

      switch (curr_filter)
      {
      case 'M':
        x = multiply(x, v);
        break;

      case 'C':
        v /= 10.0f;
        x = clip(x, v);
        break;

      case 'E':
        v /= 10.0f;
        x = equalise(x, v, w);
        break;

      case 'P':
        x = pitch_shift(x, v);
        break;

      case 'D':
        v /= 10.0f;
        x = drive(x, v);
        break;
      }
    }

    x = clip(x, 1.0f);

    push_output_record(x);

    sample = x * (SAMPLE_SCALING_FACTOR - 1);

    fwrite(&sample, sizeof(sample_size), 1, output_wav);
  }

  free_record();
}
