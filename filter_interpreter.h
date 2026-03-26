#ifndef FILTER_INTERPRETER_H
#define FILTER_INTERPRETER_H

#include "filters/util_filter_base.h"

struct filter **interpret_filter_string(const char *filter_string);
void free_filter_list(struct filter **filters);

#endif
