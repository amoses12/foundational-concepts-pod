#include "linked_list.h"

#include <stdbool.h>
#include <stdlib.h>

static ResultCode LinkedList_ItemCreate(void* payload,
                                        LinkedListItem** result) {
  LinkedListItem* item = calloc(sizeof(LinkedListItem), 1);
  if (item == NULL) return kFailedMemoryAllocation;

  item->next = NULL;
  item->prev = NULL;
  item->payload = payload;

  *result = item;
  return kSuccess;
}
static void LinkedList_ItemDestroy(LinkedList* self, LinkedListItem* doomed) {
  if (self->freer != NULL) self->freer(doomed->payload);

  free(doomed);
}

static void insert_at_head(LinkedList* list, LinkedListItem* item) {
  if (list->head == NULL) {
    list->head = item;
    list->tail = item;
    return;
  }
  item->prev = NULL;
  list->head->prev = item;
  item->next = list->head;
  list->head = item;
}

static void insert_at_tail(LinkedList* list, LinkedListItem* item) {
  item->prev = list->tail;
  list->tail->next = item;
  list->tail = item;
}

static void insert_mid(LinkedList* list, LinkedListItem* item, size_t index) {
  LinkedListItem* curr = list->head;
  for (size_t i; i < index; i++) {
    curr = curr->next;
  }
  item->prev = curr;
  item->next = curr->next;

  curr->next->prev = item;
  curr->next = item;
}

static void delete_at_head(LinkedList* list) {
  LinkedListItem* head = list->head;

  if (head == list->tail) {
    list->head = NULL;
    list->tail = NULL;
  } else {
    list->head = head->next;
  }

  if (list->head != NULL) list->head->prev = NULL;

  LinkedList_ItemDestroy(list, head);
}

static void delete_at_tail(LinkedList* list) {
  LinkedListItem* tail = list->tail;

  tail->prev->next = NULL;
  list->tail = tail->prev;

  LinkedList_ItemDestroy(list, tail);
}

static void delete_mid(LinkedList* list, LinkedListItem* doomed) {
  LinkedListItem* prev = doomed->prev;
  LinkedListItem* next = doomed->next;

  prev->next = next;
  next->prev = prev;

  LinkedList_ItemDestroy(list, doomed);
}

static void delete_at_mid(LinkedList* list, size_t index) {
  LinkedListItem* item = list->head;

  for (size_t i; i < index; i++) {
    delete_mid(list, item);
  }
}

static LinkedListItem* search(const LinkedList* self, const void* item) {
  LinkedListItem* current = self->head;

  while (current != NULL) {
    if (self->comparator(item, current->payload) == 0) return current;

    current = current->next;
  }
  return NULL;
}

ResultCode LinkedList_Create(freer freer, sort_strategy comparator,
                             LinkedList** result) {
  if (*result != NULL) return kOutputPointerIsNotNull;
  LinkedList* list = calloc(sizeof(LinkedList), 1);

  if (list == NULL) return kFailedMemoryAllocation;

  list->freer = freer;
  list->comparator = comparator;

  *result = list;
  return kSuccess;
}

ResultCode LinkedList_InsertAt(LinkedList* self, void* payload,
                               const size_t index) {
  if (self == NULL || payload == NULL) return kNullParameter;
  if (index > self->size) return kInvalidIndex;

  LinkedListItem* item = NULL;
  ResultCode result = LinkedList_ItemCreate(payload & item);
  if(result != kSuccess) return result;

  if (index == 0) {
    insert_at_head(self, item);
  } else if (index == self->size) {
    insert_at_tail(self, item);
  } else {
    insert_mid(self, item, index);
  }

  self->size++;

  return kSuccess;
}

ResultCode LinkedList_DeleteAt(LinkedList* self, const size_t index) {
  if (self == NULL) return kNullParameter;
  if (index >= self->size) return kInvalidIndex;

  if (index == 0) {
    delete_at_head(self);
  } else if (index == self->size - 1) {
    delete_at_tail(self);
  } else {
    delete_at_mid(self, index);
  }

  self->size--;
  return kSuccess;
}

ResultCode LinkedList_Delete(LinkedList* self, void* doomed) {
  if (self == NULL || doomed == NULL) return kNullParameter;

  LinkedListItem* item = search(self, doomed);
  if (item == NULL) return kNotFound;

  if (item == self->head) {
    delete_at_head(self);
  } else if (item == self->tail) {
    delete_at_tail(self);
  } else {
    delete_mid(self, item);
  }

  self->size--;
  return kSuccess;
}

ResultCode LinkedList_Search(const LinkedList* self, const void* item,
                             void** result) {
  if (self == NULL || item == NULL) return kNullParameter;
  if (*result != NULL) return kNotFound;

  LinkedListItem* found = search(self, item);
  if (found == NULL) return kNotFound;

  *result = found->payload;
  return kSuccess;
}