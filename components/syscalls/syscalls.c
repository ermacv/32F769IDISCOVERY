#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <assert.h>

#include "heap.h"
#include "SEGGER_RTT.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

extern void main(void);

static SemaphoreHandle_t log_lock;
static SemaphoreHandle_t heap_lock;
static StaticSemaphore_t log_lock_buf;
static StaticSemaphore_t heap_lock_buf;

static void syscalls_init(void) {
  heap_init();
  SEGGER_RTT_Init();
  log_lock = xSemaphoreCreateMutexStatic(&log_lock_buf);
  assert(log_lock);
  heap_lock = xSemaphoreCreateMutexStatic(&heap_lock_buf);
  assert(heap_lock);
}

static int syscall_not_implemented(struct _reent *r, ...) {
  __errno_r(r) = ENOSYS;
  return -1;
}

static int syscall_not_implemented_aborts(void) {
  abort();
}

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattribute-alias"
#endif

int _open_r(struct _reent *r, const char * path, int flags, int mode)
    __attribute__((weak,alias("syscall_not_implemented")));
int _close_r(struct _reent *r, int fd)
    __attribute__((weak,alias("syscall_not_implemented")));
off_t _lseek_r(struct _reent *r, int fd, off_t size, int mode)
    __attribute__((weak,alias("syscall_not_implemented")));
int _fcntl_r(struct _reent *r, int fd, int cmd, int arg)
    __attribute__((weak,alias("syscall_not_implemented")));
int _stat_r(struct _reent *r, const char * path, struct stat * st)
    __attribute__((weak,alias("syscall_not_implemented")));
int _link_r(struct _reent *r, const char* n1, const char* n2)
    __attribute__((weak,alias("syscall_not_implemented")));
int _unlink_r(struct _reent *r, const char *path)
    __attribute__((weak,alias("syscall_not_implemented")));
int _rename_r(struct _reent *r, const char *src, const char *dst)
    __attribute__((weak,alias("syscall_not_implemented")));
int _isatty_r(struct _reent *r, int fd)
    __attribute__((weak,alias("syscall_not_implemented")));


/* These functions are not expected to be overridden */
int _system_r(struct _reent *r, const char *str)
    __attribute__((alias("syscall_not_implemented")));
int raise(int sig)
    __attribute__((alias("syscall_not_implemented_aborts")));
int _raise_r(struct _reent *r, int sig)
    __attribute__((alias("syscall_not_implemented_aborts")));
void* _sbrk_r(struct _reent *r, ptrdiff_t sz)
    __attribute__((alias("syscall_not_implemented_aborts")));
int _getpid_r(struct _reent *r)
    __attribute__((alias("syscall_not_implemented")));
int _kill_r(struct _reent *r, int pid, int sig)
    __attribute__((alias("syscall_not_implemented")));
void _exit(int __status)
    __attribute__((alias("syscall_not_implemented_aborts")));

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif

int _read(int file, char *ptr, int len)
{
  (void)file;
  return SEGGER_RTT_Read(0, ptr, len);
}

int _write(int file, char *ptr, int len)
{
  (void)file;
  int DataIdx;

  for (DataIdx = 0; DataIdx < len; DataIdx++) {
    SEGGER_RTT_PutChar(0, *ptr++);
  }
  return len;
}

int _fstat(int file, struct stat *st)
{
  (void)file;
  st->st_mode = S_IFCHR;
  return 0;
}

int _wait(int *status)
{
  (void)status;
  errno = ECHILD;
  return -1;
}

int _times(struct tms *buf)
{
  (void)buf;
  return -1;
}

int _fork(void)
{
  errno = EAGAIN;
  return -1;
}

void *__wrap_malloc(size_t sz) {
  void *ptr;
  xSemaphoreTake(heap_lock, portMAX_DELAY);
  ptr = heap_malloc(kHEAP_TYPE_INT_RAM, sz);
  xSemaphoreGive(heap_lock);
  return ptr;
}

void *__wrap_realloc(void *p, size_t sz) {
  void *ptr;
  xSemaphoreTake(heap_lock, portMAX_DELAY);
  ptr = heap_realloc(kHEAP_TYPE_INT_RAM, p, sz);
  xSemaphoreGive(heap_lock);
  return ptr;
}

void *__wrap_calloc(size_t n, size_t sz) {
  void *ptr;
  xSemaphoreTake(heap_lock, portMAX_DELAY);
  ptr = heap_calloc(kHEAP_TYPE_INT_RAM, n, sz);
  xSemaphoreGive(heap_lock);
  return ptr;
}

void __wrap_free(void *p) {
  xSemaphoreTake(heap_lock, portMAX_DELAY);
  heap_free(kHEAP_TYPE_INT_RAM, p);
  xSemaphoreGive(heap_lock);
}

int __wrap_printf(const char * sFormat, ...) {
  // xSemaphoreTake(log_lock, portMAX_DELAY);
  va_list ap;
  va_start(ap, sFormat);
  int res = SEGGER_RTT_vprintf(0, sFormat, &ap);
  va_end(ap);
  // xSemaphoreGive(log_lock);
  return res;
}

void __assert_func (const char * file, int line, const char *func, const char *expr) {
  (void)func;
  (void)expr;
  printf("Assertion triggered. %s:%d\r\n", file, line);
  abort();
}

static void main_caller(void *arg) {
  (void)arg;
  main();
  vTaskDelete(NULL);
}

void ll_main(void) {
  syscalls_init();
  xTaskCreate(main_caller, "main", 256, NULL, 1, NULL);
  vTaskStartScheduler();
}
