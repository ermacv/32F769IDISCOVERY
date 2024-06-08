#include "FreeRTOS.h"
#include "task.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

static void ping(void *arg) {
  (void)arg;
  uint32_t counter = 0;
  while(1) {
    printf("ping = %lu\r\n", counter++);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

int main(void) {
  printf("%s   started!\r\n", __func__);
  void *data = malloc(1024);
  if (((uint8_t *)data)[0] == 0) {
    ((uint8_t *)data)[0] = 1;
  }
  free(data);

  BaseType_t rc = xTaskCreate(ping, "ping", 128, NULL, 1, NULL);
  assert(rc == pdPASS);
  return 0;
}