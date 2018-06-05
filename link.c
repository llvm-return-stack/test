#include <stdio.h>

extern int sum (int a, int b);

int main (void) {
  int s;

  s = sum(1, 2);
  if (s != 3) {
    fprintf(stderr, "main() failed: Calling library function did not succeed correctly.\n");
    return 1;
  }

  printf("OK\n");
  return 0;
}

