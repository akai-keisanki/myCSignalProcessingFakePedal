#ifndef FILTER_INTERPRETER_H
#define FILTER_INTERPRETER_H

#include <stdio.h>

#include "filters/util_filter_base.h"

struct filter **interpret_fpfdsl_files(FILE *log, const char **filter_file_names, size_t file_amount);
struct filter **interpret_filter_string(const char *filter_string);
void free_filter_list(struct filter **filters);

#endif
