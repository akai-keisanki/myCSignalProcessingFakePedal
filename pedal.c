#include "pedal.h"
#include "filter_interpreter.h"

#include "filters/clip.h"

#include <stddef.h>
#include <stdlib.h>
#include <pthread.h>

#include <portaudio.h>

#define SAMPLE_RATE (size_t)(44100)
#define PA_SAMPLE_TYPE paFloat32
#define FRAMES_PER_BUFFER (size_t)(0x2)
#define INPUT_CHANNELS 1
#define OUTPUT_CHANNELS 1

const sample_size SAMPLE_SCALING_FACTOR = 1 << (sizeof(sample_size)*8 - 1);
const float SAMPLE_16_TO_FLOAT_SCALLING_FACTOR = 1 << 15;

pthread_mutex_t filter_list_mutex = PTHREAD_MUTEX_INITIALIZER;

struct play_ctx
{
  struct filter **filter_list;
};

float apply_filters(float x, struct filter **filter_list)
{
  struct filter *clip1_filter = init_filter_clip(1.0f);

  for (size_t i = 0; filter_list[i]; ++i)
    x = apply_filter(filter_list[i], x);

  x = apply_filter(clip1_filter, x);

  free_filter(clip1_filter);

  return x;
}

void pedal_in_files(FILE *output_wav, FILE *input_wav, struct filter **filter_list)
{
  sample_size sample;
  float x;

  while (fread(&sample, sizeof(sample_size), 1, input_wav) == 1)
  {
    x = sample / (float)SAMPLE_SCALING_FACTOR;
    pthread_mutex_lock(&filter_list_mutex);
    x = apply_filters(x, filter_list);
    pthread_mutex_unlock(&filter_list_mutex);
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

bool pedal_live_loop_check_id(struct filter **filter_list, size_t id)
{
  bool avl = false;

  for (size_t n = 0; filter_list[n] && n < MAX_FILTERS_SIZE - 1; ++n)
  {
    if (n >= id)
    {
      avl = true;
      break;
    }
  }

  return avl;
}

void pedal_live_loop(struct filter **filter_list)
{
  char c;
  size_t id;
  bool loop = true, avl;

  puts("\nInput \"q\" to stop or \"h\" for help.");

  while (loop)
  {
    fputs("\nfake_pedal_live:", stdout);
    c = getchar();

    while (c == ' ' || c == '\n')
      c = getchar();

    putchar('\n');

    fflush(stdout);

    switch (c)
    {
    case 'h':
    case '?':
      puts("Input \"l\" or \"#\" to list current loaded filters");
      puts("Input \"r FILTER_ID\" or \"- FILTER_ID\" to remove all filters with ID >= FILTER_ID");
      puts("Input \"e FILTER_ID\" or \"u FILTER_ID\" or \"> FILTER_ID\" to edit a filter by ID");
      puts("Input \"a FPFDSL_FILTER\" or \"c FPFDSL_FILTER\" or \"+ FPFDSL_FILTER\" to add a filter written in FPFDSL");
      puts("Input \"q\" or \"!\" to stop");
      puts("Input \"h\" or \"?\" for help");
      break;

    case 'l':
    case '#':
      printf("%6s %10s\n", "id", "filter_label");
      for (id = 0; filter_list[id]; ++id)
        printf("%6lu %10s\n", id, get_filter_label(filter_list[id]));
      break;

    case 'r':
    case '-':
      scanf("%zu", &id);

      if (!pedal_live_loop_check_id(filter_list, id))
        puts("ID not available");
      else
      {
        pthread_mutex_lock(&filter_list_mutex);
        for (; filter_list[id]; ++id)
        {
          free_filter(filter_list[id]);
          filter_list[id] = NULL;
        }
        pthread_mutex_unlock(&filter_list_mutex);
      }

      break;

    case 'e':
    case 'u':
    case '>':
      pthread_mutex_lock(&filter_list_mutex);

      scanf("%zu", &id);

      if (!pedal_live_loop_check_id(filter_list, id))
        puts("ID not available");
      else
      {
        free_filter(filter_list[id]);
        filter_list[id] = interpret_fpfdsl_filter(stderr, stdin);
      }

      pthread_mutex_unlock(&filter_list_mutex);

      break;

    case 'a':
    case 'c':
    case '+':
      pthread_mutex_lock(&filter_list_mutex);

      for (id = 0; filter_list[id]; ++id);

      if (id > MAX_FILTERS_SIZE - 2)
      {
        puts("Too many filters.");
        break;
      }

      filter_list[id++] = interpret_fpfdsl_filter(stderr, stdin);
      filter_list[id] = NULL;

      pthread_mutex_unlock(&filter_list_mutex);

      break;

    case 'q':
    case '!':
      loop = false;
      break;

    default:
      puts("Input \"q\" to stop or \"h\" for help.");
      break;
    }
  }
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
  
  pedal_live_loop(filter_list);

  Pa_StopStream(stream);
  Pa_CloseStream(stream);
  Pa_Terminate();
}
