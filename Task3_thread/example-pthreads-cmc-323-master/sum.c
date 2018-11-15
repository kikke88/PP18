

#include <pthread.h>
#include <stdio.h>

#define NUM_THREADS 4

pthread_mutex_t lock;
pthread_cond_t cond;

void *work(void *ptr) {
  int *sum = (int *)ptr;
  for (int i = 0; i < 100000; i++) {
    pthread_mutex_lock(&lock);
    (*sum)++;
    if (*sum >= 42) {
      pthread_cond_signal(&cond);
    }
    pthread_mutex_unlock(&lock);
  }
  return NULL;
}

void *waiter(void *ptr) {
  int *sum = (int *)ptr;

  pthread_mutex_lock(&lock);
  while (*sum < 42) {
    pthread_cond_wait(&cond, &lock);
  }
  printf("*sum = %d\n", *sum);
  pthread_mutex_unlock(&lock);
  return NULL;
}

int main() {
  pthread_t threads[NUM_THREADS];

  pthread_mutex_init(&lock, NULL);
  pthread_cond_init(&cond, NULL);
  int sum = 0;

  for (long id = 0; id < NUM_THREADS - 1; id++) {
    pthread_create(threads + id, NULL, work, &sum);
  }
  pthread_create(threads + NUM_THREADS - 1, NULL, waiter, &sum);
  
  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  pthread_mutex_destroy(&lock);
  pthread_cond_destroy(&cond);
  printf("Sum = %d\n", sum);
}
