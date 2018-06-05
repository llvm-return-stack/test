#include <stdarg.h>
#include <stdio.h>

static int sum (const int count, ...) {
  int i, s;
  va_list vl;
  va_start (vl, count);

  s = 0;
  for (i = 0; i < count; ++i)
    s += va_arg(vl, int);

  va_end(vl);

  return s;
}

int main (void) {
  int s;

  s = sum(10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
  if (s != 55) {
    fprintf(stderr, "main() failed: Variadic function did not execute correctly.\n");
    return 1;
  }

  printf("OK\n");
  return 0;
}
