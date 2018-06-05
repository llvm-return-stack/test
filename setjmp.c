#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifdef DEBUG
#if defined(__aarch64__)
#define asm_read_marker(marker) \
  asm volatile ("ldr %0, [x28, #-8]" \
               : "=r" (marker))
#define asm_read_rsp(rsp) \
  asm volatile ("mov %0, x28" \
               : "=r" (rsp))
#elif defined (__x86_64__)
#define asm_read_marker(marker) \
  asm volatile ("movq (-8)(%%r15), %0" \
               : "=r" (marker))
#define asm_read_rsp(rsp) \
  asm volatile ("movq %%r15, %0" \
               : "=r" (rsp))
#endif
#endif

static void first(void);
static void second(void);
static void third(void);

static jmp_buf env;
static int handled = 0;

int main (void) {
#ifdef DEBUG
  unsigned long rsp;
  unsigned long marker;
  asm_read_rsp(rsp);
  printf("(Main) RSP=0x%lx\n", rsp);
#endif
  first();

  if (handled != 2) {
    fprintf(stderr, "main() failed: Not all setjmps succeeded.\n");
    return 1;
  }

  printf("OK\n");
  return 0;
}

static void first (void) {
#ifdef DEBUG
  unsigned long rsp;
  unsigned long marker;
  asm_read_rsp(rsp);
  asm_read_marker(marker);
  printf("(First) RSP=0x%lx\n", rsp);
  printf("(First) Marker=0x%lx\n", marker);
#endif

  if (setjmp(env)) {
    /* Handling longjmp() from second(). */
    handled++;
  } else {
    second();
  }
}

static void second (void) {
  jmp_buf my_env;

#ifdef DEBUG
  unsigned long rsp;
  unsigned long marker;
  asm_read_rsp(rsp);
  asm_read_marker(marker);
  printf("(Second) RSP=0x%lx\n", rsp);
  printf("(Second) Marker=0x%lx\n", marker);
#endif

  /* Saving env. */
  memcpy(my_env, env, sizeof my_env);

  switch (sigsetjmp(env, 1)) {
    case 3:
      /* Handling siglongjmp() from third(). */
      handled++;

    default:
      /* Restoring env. */
      memcpy(env, my_env, sizeof env);
      /* Returning to first() with status 2. */
      longjmp(env, 2);

    case 0:
      third();
  }
}

static void third (void) {
#ifdef DEBUG
  unsigned long rsp;
  unsigned long marker;
  asm_read_rsp(rsp);
  printf("(Third) RSP=0x%lx\n", rsp);
#endif

  /* Returning to second() with status 3. */
  siglongjmp(env, 3);
}
