#ifndef _PLATFORM_INTERNAL_H
#define _PLATFORM_INTERNAL_H

#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef BH_PLATFORM_PICO
#define BH_PLATFORM_PICO
#endif

/* ---- thread types (stubs — single-threaded bare-metal) ---- */
typedef int korp_tid;
typedef struct {
  int d;
} korp_mutex;
typedef struct {
  int d;
} korp_cond;
typedef int korp_thread;
typedef struct {
  int d;
} korp_rwlock;
typedef int korp_sem;

#define OS_THREAD_MUTEX_INITIALIZER {0}

#define BH_APPLET_PRESERVED_STACK_SIZE (2 * BH_KB)
#define BH_THREAD_DEFAULT_PRIORITY 5

/* ---- file/IO types (stubs — no POSIX) ---- */
typedef int os_file_handle;
typedef void *os_dir_stream;
typedef int os_raw_file_handle;
typedef int os_poll_file_handle;
typedef unsigned int os_nfds_t;
typedef int os_timespec;

static inline int os_getpagesize(void) { return 4096; }

static inline os_file_handle os_get_invalid_handle(void) { return -1; }

#endif /* _PLATFORM_INTERNAL_H */
