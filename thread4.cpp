 #include <pthread.h>
 #include <stdio.h>
 #include <unistd.h>
 #include <stdlib.h>
 
 #define NUM_THREAD 100
 
 void* thread_add(void* arg);
 void* thread_subtraction(void* arg);

long long sum;

 int main()
 {
     pthread_t t_id[NUM_THREAD];
     int i;

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
     printf("sum is %d", sum);
 }

 void* thread_add(void* arg) {
     int i = 0;
     for (; i < 999999; i++) {
         sum += i;
     }
     return NULL;
 }

 void* thread_subtraction(void* arg) {
     int i = 0;
     for (; i < 999999; i++) {
         sum -= i;
     }
     return NULL;
 }

