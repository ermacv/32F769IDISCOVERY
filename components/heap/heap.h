#pragma once

#include <stdlib.h>

typedef enum {
 kHEAP_TYPE_INT_RAM = 0,
} heap_type_t;

void heap_init(void);
void heap_deinit(void);

void *heap_malloc(heap_type_t type, size_t sz);
void *heap_realloc(heap_type_t type, void *p, size_t sz);
void *heap_calloc(heap_type_t type, size_t n, size_t sz);
void heap_free(heap_type_t type, void *p);

size_t heap_get_used_size(heap_type_t type);
size_t heap_get_max_size(heap_type_t type);
