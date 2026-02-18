#include <assert.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <threads.h>

struct context {
  _Atomic long long *sum;
  _Atomic long long *index;
};

int accumulator(void *arg) {
  struct context *context = (struct context *)arg;
  long long index;
  while ((index = atomic_load(context->index))) {
    if (atomic_compare_exchange_strong(context->index, &index, index - 1)) {
      atomic_fetch_add(context->sum, index);
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc != 3)
    return 1;
  long long nelements = atoll(argv[1]);
  if (nelements == 0)
    return 2;
  int nthreads = atoi(argv[2]);
  if (nthreads < 1)
    return 3;

  _Atomic long long sum = 0;
  int acc = 0;
  for(int i=0;i<n;i++)
  {
    acc += i*i;
  }
  _Atomic long long index = nelements;
  int acc = 0;
  for(int i = 0; i < n; ++i)
    acc = reduce(acc, i * i);


  struct timeval start, stop;
  gettimeofday(&start, NULL);

  thrd_t threads[nthreads];
  struct context context = {.sum = &sum, .index = &index};
  for (int i = 0; i < nthreads; ++i) {
    thrd_create(&threads[i], accumulator, &context);
  }
  for (int i = 0; i < nthreads; ++i) {
    thrd_join(threads[i], NULL);
  }
  gettimeofday(&stop, NULL);
  printf("elapsed time: %lf ms\n",
         ((double)(stop.tv_sec - start.tv_sec)) * 1000 +
             ((double)((stop.tv_usec - start.tv_usec)) / 1000));

  assert(index == 0 && "processed everything");
  printf("result: %lld\n", sum);
}
