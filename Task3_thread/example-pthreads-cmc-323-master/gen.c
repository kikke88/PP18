

#include "queue.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS 4

struct ctx_t {
  int id;
  queue_t *q;
  pthread_cond_t *cond;
  pthread_mutex_t *lock;
};

enum event_type_t {
  SQUARE,
  DONE,
};

struct event_t {
  enum event_type_t type;
  int value;
};

struct event_t *event_init(enum event_type_t type, int value) {
  struct event_t *ret = calloc(1, sizeof(struct event_t));
  assert(ret);
  ret->type = type;
  ret->value = value;
  return ret;
}

void event_destroy(struct event_t *ev) { free(ev); }

void *work(void *context) {
  struct ctx_t *ctx = context;
  for (;;) {
    pthread_mutex_lock(ctx->lock);
    while (queue_len(ctx->q) == 0) {
      pthread_cond_wait(ctx->cond, ctx->lock);
    }
    struct event_t *ev = queue_pop(ctx->q);
    pthread_mutex_unlock(ctx->lock);

    switch (ev->type) {
    case SQUARE:
      printf("I'm %d; result=%d\n", ctx->id, ev->value * ev->value);
      event_destroy(ev);
      break;
    case DONE:
      event_destroy(ev);
      return NULL;
      break;
    }
  }
}

void *gen(void *context) {
  struct ctx_t *ctx = context;
  for (int i = 0; i < 10; i++) {
    struct event_t *ev = event_init(SQUARE, i);
    pthread_mutex_lock(ctx->lock);
    queue_push(ctx->q, ev);
    pthread_cond_broadcast(ctx->cond);
    pthread_mutex_unlock(ctx->lock);
  }
  for (int i = 0; i < NUM_THREADS - 1; i++) {
    struct event_t *ev = event_init(DONE, 0);
    queue_push(ctx->q, ev);
  }
}

int main() {
  pthread_t threads[NUM_THREADS];

  queue_t *q = queue_init();

  pthread_cond_t cond;
  pthread_mutex_t lock;

  pthread_cond_init(&cond, NULL);
  pthread_mutex_init(&lock, NULL);

  struct ctx_t ctx[NUM_THREADS];

  for (int i = 0; i < NUM_THREADS; i++) {
    ctx[i].q = q;
    ctx[i].id = i;
    ctx[i].lock = &lock;
    ctx[i].cond = &cond;
  }

  for (long id = 0; id < NUM_THREADS - 1; id++) {
    pthread_create(threads + id, NULL, work, ctx + id);
  }
  pthread_create(threads + NUM_THREADS - 1, NULL, gen, ctx + NUM_THREADS - 1);

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  pthread_cond_destroy(&cond);
  pthread_mutex_destroy(&lock);
  queue_destroy(q);
  return 0;
}
