#include <stdint.h>
#include <stdio.h>

#ifndef FILTERS_PEDAL_H
#define FILTERS_PEDAL_H

typedef int16_t sample_size;

struct records;

struct records *init_records(void);
void free_records(struct records *records);

float apply_filters(struct records *records, float x, const char *filter_list);

void pedal_in_files(FILE *output_wav, FILE *input_wav, const char *filter_list);

#endif
