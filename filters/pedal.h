#include <stdint.h>
#include <stdio.h>

#ifndef FILTERS_PEDAL_H
#define FILTERS_PEDAL_H

typedef int16_t sample_size;

void apply_filters(FILE *output_wav, FILE *input_wav, char *filter_list);

#endif
