/*
 * WAMR platform implementation for Raspberry Pi Pico SDK (bare-metal).
 * Supports: rp2040 (Cortex-M0+), rp2350 (Cortex-M33), rp2350-riscv (Hazard3).
 */

#include "platform_api_extension.h"
#include "platform_api_vmcore.h"

#include "pico/stdlib.h"
#include "pico/time.h"

/* ---- init ---- */

int bh_platform_init(void) { return 0; }

void bh_platform_destroy(void) {}

/* ---- printf ---- */

int os_printf(const char *format, ...) {
  int ret;
  va_list ap;
  va_start(ap, format);
#ifndef BH_VPRINTF
  ret = vprintf(format, ap);
#else
  ret = BH_VPRINTF(format, ap);
#endif
  va_end(ap);
  return ret;
}

int os_vprintf(const char *format, va_list ap) {
#ifndef BH_VPRINTF
  return vprintf(format, ap);
#else
  return BH_VPRINTF(format, ap);
#endif
}

/* ---- time ---- */

uint64 os_time_get_boot_us(void) { return (uint64)time_us_64(); }

uint64 os_time_thread_cputime_us(void) { return os_time_get_boot_us(); }

/* ---- memory allocator ---- */

void *os_malloc(unsigned size) { return malloc(size); }

void *os_realloc(void *ptr, unsigned size) { return realloc(ptr, size); }

void os_free(void *ptr) { free(ptr); }

/* ---- mmap (interpreter uses malloc for linear memory; SRAM is executable)
 * ---- */

void *os_mmap(void *hint, size_t size, int prot, int flags,
              os_file_handle file) {
  (void)hint;
  (void)prot;
  (void)flags;
  (void)file;
  return malloc(size);
}

void os_munmap(void *addr, size_t size) {
  (void)size;
  free(addr);
}

int os_mprotect(void *addr, size_t size, int prot) {
  (void)addr;
  (void)size;
  (void)prot;
  return 0;
}

void *os_mremap(void *old_addr, size_t old_size, size_t new_size) {
  return os_mremap_slow(old_addr, old_size, new_size);
}

/* ---- cache ---- */

void os_dcache_flush(void) {
#if defined(__ARM_ARCH)
  __asm volatile("dsb sy" ::: "memory");
#elif defined(__riscv)
  __asm volatile("fence rw,rw" ::: "memory");
#endif
}

void os_icache_flush(void *start, size_t len) {
  (void)start;
  (void)len;
  /* Pipeline/prefetch flush after writing AOT code to SRAM. */
#if defined(__ARM_ARCH)
  __asm volatile("dsb sy\n\t isb" ::: "memory");
#elif defined(__riscv)
  __asm volatile("fence.i" ::: "memory");
#endif
}

/* ---- thread stubs (single-threaded) ---- */

korp_tid os_self_thread(void) { return 0; }

/*
 * Lowest valid stack address, defined in Pico SDK linker scripts.
 * rp2040:      stack at end of RAM,    __StackBottom = __StackTop - stack size
 * rp2350 ARM:  stack in SCRATCH_Y,     __StackBottom = __StackTop - stack size
 * rp2350 RISC-V: same convention
 */
extern char __StackBottom;

uint8 *os_thread_get_stack_boundary(void) { return (uint8 *)&__StackBottom; }

void os_thread_jit_write_protect_np(bool enabled) { (void)enabled; }

int os_thread_create(korp_tid *p_tid, thread_start_routine_t start, void *arg,
                     unsigned int stack_size) {
  (void)p_tid;
  (void)start;
  (void)arg;
  (void)stack_size;
  return -1;
}

int os_thread_create_with_prio(korp_tid *p_tid, thread_start_routine_t start,
                               void *arg, unsigned int stack_size, int prio) {
  (void)p_tid;
  (void)start;
  (void)arg;
  (void)stack_size;
  (void)prio;
  return -1;
}

int os_thread_join(korp_tid thread, void **retval) {
  (void)thread;
  (void)retval;
  return -1;
}

int os_thread_detach(korp_tid thread) {
  (void)thread;
  return -1;
}

void os_thread_exit(void *retval) { (void)retval; }

/* ---- mutex stubs (no contention in single-threaded context) ---- */

int os_mutex_init(korp_mutex *m) {
  (void)m;
  return 0;
}
int os_mutex_destroy(korp_mutex *m) {
  (void)m;
  return 0;
}
int os_mutex_lock(korp_mutex *m) {
  (void)m;
  return 0;
}
int os_mutex_unlock(korp_mutex *m) {
  (void)m;
  return 0;
}

/* ---- cond stubs ---- */

int os_cond_init(korp_cond *c) {
  (void)c;
  return 0;
}
int os_cond_destroy(korp_cond *c) {
  (void)c;
  return 0;
}
int os_cond_signal(korp_cond *c) {
  (void)c;
  return 0;
}
int os_cond_broadcast(korp_cond *c) {
  (void)c;
  return 0;
}

int os_cond_wait(korp_cond *cond, korp_mutex *mutex) {
  (void)cond;
  (void)mutex;
  return -1;
}

int os_cond_reltimedwait(korp_cond *cond, korp_mutex *mutex, uint64 useconds) {
  (void)cond;
  (void)mutex;
  (void)useconds;
  return -1;
}

/* ---- rwlock stubs ---- */

int os_rwlock_init(korp_rwlock *l) {
  (void)l;
  return 0;
}
int os_rwlock_rdlock(korp_rwlock *l) {
  (void)l;
  return 0;
}
int os_rwlock_wrlock(korp_rwlock *l) {
  (void)l;
  return 0;
}
int os_rwlock_unlock(korp_rwlock *l) {
  (void)l;
  return 0;
}
int os_rwlock_destroy(korp_rwlock *l) {
  (void)l;
  return 0;
}

/* ---- semaphore stubs ---- */

int os_sem_init(korp_sem *s, unsigned int c) {
  (void)s;
  (void)c;
  return -1;
}
int os_sem_destroy(korp_sem *s) {
  (void)s;
  return -1;
}
int os_sem_wait(korp_sem *s) {
  (void)s;
  return -1;
}
int os_sem_post(korp_sem *s) {
  (void)s;
  return -1;
}

int os_sem_reltimedwait(korp_sem *sem, uint64 useconds) {
  (void)sem;
  (void)useconds;
  return -1;
}

/* ---- sleep ---- */

int os_usleep(uint32 usec) {
  sleep_us(usec);
  return 0;
}
