#include <stdio.h>

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

struct Exception {};

void raise (void) {
#ifdef DEBUG
  unsigned long rsp;
  asm_read_rsp(rsp);
  printf("(Raise) RSP=0x%lx\n", rsp);
#endif
  throw Exception();
}

int main (void) {
  bool handled = false;
#ifdef DEBUG
  unsigned long rsp;
  asm_read_rsp(rsp);
  printf("(Main) RSP=0x%lx\n", rsp);
#endif

  try {
    raise();
  } catch(Exception &) {
    handled = true;
  }

  if (!handled) {
    fprintf(stderr, "main() failed: catch-clause was not executed correctly.\n");
    return 1;
  }

  printf("OK\n");
  return 0;
}

