#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

__attribute__((noinline)) static void task(int data) {
  struct timespec duration = {.tv_sec = 0, .tv_nsec = data};
  nanosleep(&duration, NULL);
}

__attribute__((noinline)) static void work(const int *data, int data_size) {
  for (int i = 0; i < data_size; ++i) {
    task(data[i]);
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2)
    return 1;

  int data_size = atoi(argv[1]);
  if (data_size == 0)
    return 2;

  int *data = malloc(sizeof(int) * data_size);

  // Create irregular workload.
  srand(0);
  struct timeval start, stop;
  for (int i = 0; i < data_size; ++i)
    data[i] = random() % 100000000l;

  // Process it.
  gettimeofday(&start, NULL);
  work(data, data_size);
  gettimeofday(&stop, NULL);
  printf("elapsed time: %lf ms\n",
         ((double)(stop.tv_sec - start.tv_sec)) * 1000 +
             ((double)((stop.tv_usec - start.tv_usec)) / 1000));

  return 0;
}
