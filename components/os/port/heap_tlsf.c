#include <assert.h>
#include "FreeRTOS.h"
#include "task.h"
#include "heap.h"

void *pvPortMalloc( size_t xWantedSize ) {
	return heap_malloc(kHEAP_TYPE_INT_RAM, xWantedSize);
}

void vPortFree( void *pv ) {
	heap_free(kHEAP_TYPE_INT_RAM, pv);
}

size_t xPortGetFreeHeapSize( void ) {
	size_t max_size = heap_get_max_size(kHEAP_TYPE_INT_RAM);
	size_t used_size = heap_get_used_size(kHEAP_TYPE_INT_RAM);
	assert(max_size >= used_size);
	return max_size - used_size;
}

size_t xPortGetMinimumEverFreeHeapSize( void ) {
	//! @todo Not implemented
	return 0;
}

void vPortInitialiseBlocks( void ) {
	/* This just exists to keep the linker quiet. */
}
