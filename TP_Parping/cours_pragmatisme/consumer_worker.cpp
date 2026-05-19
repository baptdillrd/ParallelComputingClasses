#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct stack {
  long value;
  struct stack *prev;
};

typedef struct stack *stack_t;

bool stack_empty(stack_t const *this) { return *this == NULL; }

long stack_head(stack_t const *this) {
  assert(!stack_empty(this) && "non-empty stack");
  return (*this)->value;
}

void stack_push(stack_t *this, long value) {
  stack_t next = malloc(sizeof(struct stack));
  next->value = value;
  next->prev = *this;
  *this = next;
}

void stack_pop(stack_t *this) {
  stack_t old = *this;
  *this = (*this)->prev;
  free(old);
}

/*
 * Repeatedly add items to the stack
 */
void *stack_writer(void *args) {
  stack_t *stack = *(stack_t **)args;
  struct timespec delay = {.tv_sec = 0, .tv_nsec = 1000000};

  long item_count = 0;
  while (true) {
    nanosleep(&delay, NULL);
    stack_push(stack, item_count);
    printf("pushed: %ld\n", item_count);
    ++item_count;
  }
}

/*
 * Repeatedly read items from the stack
 */
void *stack_reader(void *args) {
  stack_t *stack = *(stack_t **)args;
  struct timespec delay = {.tv_sec = 0, .tv_nsec = 1000000};

  while (true) {
    nanosleep(&delay, NULL);
    if (!stack_empty(stack)) {
      long value = stack_head(stack);
      printf("read: %ld\n", value);
      stack_pop(stack);
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc != 1)
    return 1;

  stack_t stack = NULL;

  stack_t *shared_stack = &stack;

  pthread_t writer, reader;

  pthread_create(&writer, NULL, stack_writer, &shared_stack);
  pthread_create(&reader, NULL, stack_reader, &shared_stack);

  pthread_join(writer, NULL);
  pthread_join(reader, NULL);

  return 0;
}
