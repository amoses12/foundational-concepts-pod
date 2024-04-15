#pragma once

#include <stddef.h>

#include "../utils/result_code.h"
#include "../utils/comparators.h"


typedef void (*item_handler)(void* x);
typedef struct {
  sort_strategy comparator; // pointer (see function definition);
  size_t n; // actual value
  size_t item_size; // actual value
  void* array; // pointer
} Array;


ResultCode Array_Create(sort_strategy, size_t item_size, Array**);