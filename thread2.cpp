#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

using namespace std;

void* thread_main(void* arg);

int main(int argc, char* argv[])
{
    pthread_t t_id;
    int thread_param = 5;
    void* status;

    if (pthread_create(&t_id, NULL, thread_main, (void*)&thread_param) != 0) {
        puts("thread create error");
        exit(-1);
    }
    cout << "thread begin" << endl;
    int err = pthread_join(t_id, &status);
    if (err != 0) {
        puts("pthread_join error");
        exit(-1);
    }
    cout << "thread return msg :" << (char*)status << endl;
    puts("end of main");
    return 0;
}

void* thread_main(void* arg)
{
    int i;
    int cnt = *((int*)arg);
    for (i = 0; i < cnt; i++) {
        sleep(1);
        puts("running thread");
    }
    return (void*)("thread end");
}