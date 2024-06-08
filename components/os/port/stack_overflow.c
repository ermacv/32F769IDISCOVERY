#include "FreeRTOS.h"
#include "task.h"

void vApplicationStackOverflowHook( TaskHandle_t xTask, char * pcTaskName ) {
  (void)xTask;
  (void)pcTaskName;
  //! @todo Not implemented
}
