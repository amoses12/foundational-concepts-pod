#pragma once

#include <stddef.h>

typedef int (*sort_strategy)(const void* x, const void* y);
typedef void (*freer)(void* x);

// Add comparator type definitions as we build comparators
int PIntComparator(const void* x, const void* y);