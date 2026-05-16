#ifndef FILTER_INTERPRETER_H
#define FILTER_INTERPRETER_H

#include <stdio.h>

#include "filters/util_filter_base.h"

#define MAX_FILTERS_SIZE (size_t)0x40

struct filter *interpret_fpfdsl_filter(FILE *log, FILE *file);
struct filter **interpret_fpfdsl_files(FILE *log, const char **filter_file_names, size_t file_amount);
struct filter **interpret_filter_string(const char *filter_string);
void free_filter_list(struct filter **filters);

#endif
