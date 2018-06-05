#include <pthread.h>
#include <stdio.h>

#define NUM_THREADS 100

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

static int counter = 0;

void *main_thread (void *t) {
#ifdef DEBUG
  unsigned long rsp;
  asm_read_rsp(rsp);
  printf("[Thread#%ld] RSP=0x%lx\n", (long)t, rsp);
#endif
  __atomic_fetch_add(&counter, 1, __ATOMIC_SEQ_CST);
  pthread_exit(NULL);
  return NULL;
}

int main (void) {
  pthread_t threads[NUM_THREADS];
  long t;

#ifdef DEBUG
  unsigned long rsp;
  asm_read_rsp(rsp);
  printf("[Main] RSP=0x%lx\n", rsp);
#endif

  for(t = 0; t < NUM_THREADS; ++t) {
    if (pthread_create(&threads[t], NULL, main_thread, (void *)t)) {
      fprintf(stderr, "pthread_create() failed: %m\n");
      return 1;
    }
  }

  for (t = 0; t < NUM_THREADS; ++t) {
    if (pthread_join(threads[t], NULL)) {
      fprintf(stderr, "pthread_join() failed: %m\n");
      return 1;
    }
  }

  if (counter != NUM_THREADS) {
    fprintf(stderr, "main() failed: Not all threads were executed.\n");
    return 1;
  }

  printf("OK\n");
  return 0;
}

