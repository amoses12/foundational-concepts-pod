#include "sorted_array.h"

#include <stdio.h>
#include <sys/_types/_null.h>

// QUESTION: need to make sure I understand binary search. Do we start in the middle and then work back and forth right/left?
static ResultCode BinarySearch(const void* arr, sort_strategy comparator,
                               const void* search_for, const size_t n,
                               const size_t item_size, void** result) {
  if (n <= 0) return kNotFound;

  size_t half = n >> 1; // QUESTION need help with this operator

  void* tester = (char*)arr + (half * item_size);
  int comp_result = comparator(search_for, tester);

  if (comp_result == 0) {
    *result = tester;
    return kSuccess;
  }

  if (comp_result > 0) {
    void* starting_point = (char*)arr + (item_size * (half + 1));
    size_t right_items = n - half - 1;
    return BinarySearch(starting_point, comparator, search_for, right_items, item_size, result);
  }

  return BinarySearch(arr, comparator, search_for, half, item_size, result);
}

static size_t CalculateIndex(const Array* sut, void* ptr) {
  size_t diff = (char*)ptr - (char*)sut->array;
  return diff / sut->item_size;
}

ResultCode SortedArray_Search(const Array* self, const void* item,
                              void** result) {
  if (self == NULL || item == NULL) return kNullParameter;

  return BinarySearch(self->array, self->comparator, item, self->n,
                      self->item_size, result);
}



ResultCode SortedArray_Min(const Array* self, void** result) {
  if (self == NULL) return kNullParameter;
  if (self->n == 0) return kEmpty;

  *result = self->array; // QUESTION: How do we assign the number of bits/bytes here? do we not need item_size?
  return kSuccess;
}

ResultCode SortedArray_Max(const Array* self, void** result) {
  if (self == NULL) return kNullParameter;
  if (self->n == 0) return kEmpty;

  *result = (char*)self->array + (self->item_size * (self->n - 1)); // QUESTION: remind me why we use the char*
  return kSuccess;
}

ResultCode SortedArray_Predecessor(const Array* self, const void* item, void** result) {
  if (self == NULL || item == NULL) return kNullParameter;
  if (self->n == 0) return kEmpty;

  void* found = NULL;
  ResultCode result_code = SortedArray_Search(self, item, &found);
  if (result_code != kSuccess) return result_code;

  if (found == self->array)
    return kInvalidIndex;  // QUESTION: is this because it has no predecessor or
                           // because nothing lives at the first index?

  *result = (char*)found - self->item_size; // take the found element, go back one unit (item_size) for predecessor;
  return kSuccess;
}

ResultCode SortedArray_Successor(const Array* self, const void* item,
                                 void** result) {
  if (self == NULL || item == NULL) return kNullParameter;
  if (self->n == 0) return kEmpty;

  void* found = NULL;
  ResultCode result_code = SortedArray_Search(self, item, &found);
  if (result_code != kSuccess) return result_code;

  if (found == (char*)self->array + (self->item_size * (self->n - 1))) {
    return kInvalidIndex;
  } 

  *result = (char*)found + self->item_size;
  return kSuccess;
}

ResultCode SortedArray_Select(const Array* self, const size_t index,
                              void** result) {
  if (self == NULL) return kNullParameter;
  if (index >= self->n) return kInvalidIndex;

  *result = (char*)self->array + (self->item_size * index);
  return kSuccess;
}

// find item and return index
ResultCode SortedArray_Rank(const Array* self, const void* item,
                            size_t* result) {
  if (self == NULL || item == NULL) return kNullParameter;
  if (self->n == 0) return kEmpty;

  void* found = NULL;
  ResultCode result_code = SortedArray_Search(self, item, &found);
  if (result_code != kSuccess) return result_code;

  *result = CalculateIndex(self, found);
  return kSuccess;
  
}