#ifndef _TARGET_H_
#define _TARGET_H_

#include "FreeRTOS.h"
#include "semphr.h"

#define TLSF_MLOCK_T            SemaphoreHandle_t
#define TLSF_CREATE_LOCK(l, b)  l = xSemaphoreCreateMutexStatic(b)
#define TLSF_DESTROY_LOCK(l)    vSemaphoreDelete(l)
#define TLSF_ACQUIRE_LOCK(l)    xSemaphoreTake (l, portMAX_DELAY)
#define TLSF_RELEASE_LOCK(l)    xSemaphoreGive(l)

#endif
