#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFERSIZE 100
#define MAX_CLINET 256

void* handle_clnt(void* arg);
void send_msg(char* msg, int len);
void error_handling(char* msg);

int client_cnt = 0;
int client_socks[MAX_CLINET];
pthread_mutex_t mutex;

int main()
{
    int serverSock, clientSock;
    sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrSize;
    pthread_t t_id;

    pthread_mutex_init(&mutex, NULL);
    serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0) {
        fputs("serverSock create error", stderr);
        exit(-1);
    }

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8090);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int err = bind(serverSock, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (err < 0) {
        fputs("bind error", stderr);
        exit(-1);
    }
    err = listen(serverSock, 5);
    if (err < 0) {
        fputs("listen error", stderr);
        exit(-1);
    }
    while (1) {
        clientAddrSize = sizeof(clientAddr);
        clientSock = accept(serverSock, (sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSock < 0) {
            fputs("clientSock accept error", stderr);
            exit(-1);
        }
        pthread_mutex_lock(&mutex);
        client_socks[client_cnt++] = clientSock;
        pthread_mutex_unlock(&mutex);

        pthread_create(&t_id, NULL, handle_clnt, (void*)&clientSock);
        pthread_detach(t_id);
        printf("connected ip : %s\n", inet_ntoa(clientAddr.sin_addr));
    }
    close(serverSock);
    return 0;
}

void* handle_clnt(void* arg)
{
    int clientSock = *((int*)arg);
    int str_len = 0, i;
    char msg[BUFFERSIZE];
    while ((str_len = read(clientSock, msg, BUFFERSIZE))!= 0) {
        send_msg(msg, str_len);
    }
    pthread_mutex_lock(&mutex);
    for (i = 0; i < client_cnt; i++) {
        if (clientSock == client_socks[i]) {
            while (i++ < client_cnt - 1) {
                client_socks[i] = client_socks[i + 1];
            }
            break;
        }
    }
    client_cnt--;
    pthread_mutex_unlock(&mutex);
    close(clientSock);
}

void send_msg(char* msg, int len)
{
    int i;
    pthread_mutex_lock(&mutex);
    for (i = 0; i < client_cnt; i++) {
        write(client_socks[i], msg, len);
    }
    pthread_mutex_unlock(&mutex);
}
