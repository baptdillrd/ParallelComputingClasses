#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>

__attribute__((noinline)) static void task(int data) {
  struct timespec duration = {.tv_sec = 0, .tv_nsec = data};
  nanosleep(&duration, NULL);
}

struct context{
  int start;
  int stop;
  const int* data;
};

void* thread_task(void* arg) {
  struct context* ctx = arg;
  int start = ctx->start;
  int stop = ctx->stop;
  const int* data = ctx->data;

  for (int i = 0; i < data_size; ++i)
  {
    task(data[i]);
  }
  return NULL;
}

int min(int x, int y) {
  return x < y ? x :y;
}

const int nbthread = 4;

__attribute__((noinline)) static void work(const int *data, int data_size) {
  pthread_t (nbthread);

  struct context ctx[nbthread];
  for(int i = 0; i<nbthread; i++)
  {
    ctx[i].start = i * data_size/nbthread;
    ctx[i].stop = min((i+1)* data_size/nbthread, data_size);
    ctx[i].data = data;
    pthread_create(&tid[i], NULL, thread_task, &ctx[i]);
  }
  gettimeofday(&stop, NULL);
  

  ctx.start = 0;
  ctx.stop = stop;

  ctx.data = data;

  pthread_create(&tid, NULL, thread_task, &ctx);

  pthread_join(tid, NULL);
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
