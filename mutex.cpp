#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREAD 100

void* thread_add(void* arg);
void* thread_subtraction(void* arg);

long long sum;
pthread_mutex_t mutex;

int main()
{
    pthread_t t_id[NUM_THREAD];
    int i;

    pthread_mutex_init(&mutex, NULL);


    for (i = 0; i < NUM_THREAD; i++) {
        if (i % 2) {
            pthread_create(&t_id[i], NULL, thread_add, NULL);
        } else {
            pthread_create(&t_id[i], NULL, thread_subtraction, NULL);
        }
    }

    for (i = 0; i < NUM_THREAD; i++) {
        pthread_join(t_id[i], NULL);
    }
    printf("sum is %d\n", sum);
    pthread_mutex_destroy(&mutex);
    return 0;
}

void* thread_add(void* arg)
{
    int i = 0;
    pthread_mutex_lock(&mutex);
    for (; i < 999999; i++) {
        sum += i;
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* thread_subtraction(void* arg)
{
    int i = 0;
    pthread_mutex_lock(&mutex);
    for (; i < 999999; i++) {
        sum -= i;
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}
