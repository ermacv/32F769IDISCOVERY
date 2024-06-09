#include "heap.h"
#include "tlsf.h"
#include <assert.h>

#define kHEAP_BUFFER_RAM_SZ (448 * 1024)
#define kHEAP_BUFFER_SDRAM_SZ (16 * 1024 * 1024)

static uint8_t heap_buffer_ram[kHEAP_BUFFER_RAM_SZ];
static uint8_t heap_buffer_sdram[kHEAP_BUFFER_SDRAM_SZ] __attribute__((section(".SDRAM_HEAP")));

void heap_init(void) {
  size_t sz = init_memory_pool(kHEAP_BUFFER_RAM_SZ, heap_buffer_ram);
  assert(sz != (size_t)(-1));
  sz = init_memory_pool(kHEAP_BUFFER_SDRAM_SZ, heap_buffer_sdram);
  assert(sz != (size_t)(-1));
}

void heap_deinit(void) {
  destroy_memory_pool(heap_buffer_ram);
}

void *heap_malloc(heap_type_t type, size_t sz) {
  assert(type == kHEAP_TYPE_INT_RAM);
  return malloc_ex(sz, heap_buffer_ram);
}

void *heap_realloc(heap_type_t type, void *p, size_t sz) {
  assert(type == kHEAP_TYPE_INT_RAM);
  return realloc_ex(p, sz, heap_buffer_ram);
}

void *heap_calloc(heap_type_t type, size_t n, size_t sz) {
  assert(type == kHEAP_TYPE_INT_RAM);
  return calloc_ex(n, sz, heap_buffer_ram);
}

void heap_free(heap_type_t type, void *p) {
  assert(type == kHEAP_TYPE_INT_RAM);
  return free_ex(p, heap_buffer_ram);
}

size_t heap_get_used_size(heap_type_t type) {
  assert(type == kHEAP_TYPE_INT_RAM);
  return get_used_size(heap_buffer_ram);
}

size_t heap_get_max_size(heap_type_t type) {
  assert(type == kHEAP_TYPE_INT_RAM);
  return get_max_size(heap_buffer_ram);
}
