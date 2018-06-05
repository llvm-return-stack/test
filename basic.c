#include <stdio.h>

#define MAX_CALL_DEPTH 10

#ifdef DEBUG
#if defined(__aarch64__)
#define asm_read_rsp(rsp) \
  asm volatile ("mov %0, x28" \
               : "=r" (rsp))
#elif defined (__x86_64__)
#define asm_read_rsp(rsp) \
  asm volatile ("movq %%r15, %0" \
               : "=r" (rsp))
#endif
#endif

static int callee (int depth) {
#ifdef DEBUG
  unsigned long rsp;
  asm_read_rsp(rsp);
  printf("(Callee#%d) RSP=0x%lx\n", depth, rsp);
#endif
  depth++;
  if (depth != MAX_CALL_DEPTH)
    return callee(depth);
  return depth;
}

int main (void) {
  int depth;
#ifdef DEBUG
  unsigned long rsp;
  asm_read_rsp(rsp);
  printf("(Main) RSP=0x%lx\n", rsp);
#endif

  depth = callee(0);
  if (depth != MAX_CALL_DEPTH) {
    fprintf(stderr, "main() failed: Not all callees were executed.\n");
    return 1;
  }

  printf("OK\n");
  return 0;
}

