#include <stdio.h>

#if defined(__aarch64__)
#define asm_read_rsp(rsp) \
  asm volatile ("mov %0, x28" \
               : "=r" (rsp))
#elif defined (__x86_64__)
#define asm_read_rsp(rsp) \
  asm volatile ("movq %%r15, %0" \
               : "=r" (rsp))
#endif

static long tail (unsigned long old) {
  unsigned long rsp;
  asm_read_rsp(rsp);
#ifdef DEBUG
  printf("[Tail] RSP=0x%lx\n", rsp);
#endif
  return (long)(old - rsp);
}

static long caller (void) {
  unsigned long rsp;
  asm_read_rsp(rsp);
#ifdef DEBUG
  printf("(Caller) RSP=0x%lx\n", rsp);
#endif
  return tail(rsp);
}

int main (void) {
  int rc;
#ifdef DEBUG
  unsigned long rsp;
  asm_read_rsp(rsp);
  printf("(Main) RSP=0x%lx\n", rsp);
#endif

  rc = caller();
  if (rc != 0) {
    fprintf(stderr, "main() failed: Tail call did not succeed.\n");
    return 1;
  }

  printf("OK\n");
  return 0;
}

