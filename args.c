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

static long callee (long a, long b, long c, long d, long e, long f, long g,
                    long h, long i, long j, long k, long l, long m, long n,
                    long o, long p) {
  long q, r, s;

#ifdef DEBUG
  unsigned long rsp;
  asm_read_rsp(rsp);
  printf("(Callee) RSP=0x%lx\n", rsp);
#endif

  q = a + b + c + d + e + f;
  r = g + h + i + j + k + l;
  s = m + n + o + p + q + r;

  return s;
}

int main (void) {
  long a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, s;

#ifdef DEBUG
  unsigned long rsp;
  asm_read_rsp(rsp);
  printf("(Main) RSP=0x%lx\n", rsp);
#endif

  a = 0x0000000000000000L;
  b = 0x1000000000000000L;
  c = 0x1100000000000000L;
  d = 0x1110000000000000L;
  e = 0x1111000000000000L;
  f = 0x1111100000000000L;
  g = 0x1111110000000000L;
  h = 0x1111111000000000L;
  i = 0x1111111100000000L;
  j = 0x1111111110000000L;
  k = 0x1111111111000000L;
  l = 0x1111111111100000L;
  m = 0x1111111111110000L;
  n = 0x1111111111111000L;
  o = 0x1111111111111100L;
  p = 0x1111111111111110L;

  s = callee(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p);
  if (s != 0xfedcba9876543210L) {
    fprintf(stderr, "main() failed: The sum of arguments was not computed correctly.\n");
    return 1;
  }

  printf("OK\n");
  return 0;
}

