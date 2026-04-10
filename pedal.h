#ifndef FILTERS_PEDAL_H
#define FILTERS_PEDAL_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "filters/util_filter_base.h"

typedef int16_t sample_size;

float apply_filters(float x, struct filter **filter_list);

void pedal_in_files(FILE *output_wav, FILE *input_wav, struct filter **filter_list);

void pedal_live(struct filter **filter_list);

#endif
