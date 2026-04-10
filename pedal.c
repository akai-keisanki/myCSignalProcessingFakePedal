#include "pedal.h"

#include <stddef.h>
#include <stdlib.h>

#include <portaudio.h>

#define SAMPLE_RATE (size_t)(44100)
#define PA_SAMPLE_TYPE paFloat32
#define FRAMES_PER_BUFFER (size_t)(0x1)
#define INPUT_CHANNELS 1
#define OUTPUT_CHANNELS 1

const sample_size SAMPLE_SCALING_FACTOR = 1 << (sizeof(sample_size)*8 - 1);
const float SAMPLE_16_TO_FLOAT_SCALLING_FACTOR = 1 << 15;

struct play_ctx
{
  struct filter **filter_list;
};

float apply_filters(float x, struct filter **filter_list)
{
  for (size_t i = 0; filter_list[i]; ++i)
    x = apply_filter(filter_list[i], x);

  return x;
}

void pedal_in_files(FILE *output_wav, FILE *input_wav, struct filter **filter_list)
{
  sample_size sample;
  float x;

  while (fread(&sample, sizeof(sample_size), 1, input_wav) == 1)
  {
    x = sample / (float)SAMPLE_SCALING_FACTOR;
    x = apply_filters(x, filter_list);
    sample = x * (SAMPLE_SCALING_FACTOR - 1);

    fwrite(&sample, sizeof(sample_size), 1, output_wav);
  }
}

signed int pedal_callback(
    const void *inputBuffer,
    void *outputBuffer,
    size_t framesPerBuffer,
    const PaStreamCallbackTimeInfo *timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData
  )
{
  struct play_ctx *ctx = userData;
  const float *input = inputBuffer;
  float *output = outputBuffer;

  if (!input)
    for (size_t i = 0; i < framesPerBuffer; ++i)
      output[i] = 0.0f;
  else
    for (size_t i = 0; i < framesPerBuffer; ++i)
      output[i] = apply_filters(input[i], ctx->filter_list);

  return paContinue;
}

void pedal_live(struct filter **filter_list)
{
  PaStream *stream;
  struct play_ctx ctx = {filter_list};

  Pa_Initialize();

  Pa_OpenDefaultStream(
      &stream,
      INPUT_CHANNELS,
      OUTPUT_CHANNELS,
      PA_SAMPLE_TYPE,
      SAMPLE_RATE,
      FRAMES_PER_BUFFER,
      pedal_callback,
      &ctx
    );

  Pa_StartStream(stream);
  
  puts("Press ENTER to stop.");
  getchar();

  Pa_StopStream(stream);
  Pa_CloseStream(stream);
  Pa_Terminate();
}
