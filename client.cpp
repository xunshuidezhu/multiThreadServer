#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define NAMESIZE 20
#define BUFSIZE 100

void* send_msg(void* arg);
void* recv_msg(void* arg);
void error_handling(char* msg);

char name[NAMESIZE] = "<default>";
char msg[BUFSIZE];

void error_handling(char* msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(-1);
}

int main(int argc, char* argv[])
{
    int clientSock;
    sockaddr_in serverAddr;
    pthread_t send_thread, recv_thread;
    void* thread_return;
    sprintf(name, "<%s>", argv[1]);
    clientSock = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSock < 0) {
        error_handling("clientSock create error");
    }

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8090);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    if ((connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)) {
        error_handling("clientSock connect error");
    }

    pthread_create(&send_thread, NULL, send_msg, (void*)&clientSock);
    pthread_create(&send_thread, NULL, recv_msg, (void*)&clientSock);
    pthread_join(send_thread, &thread_return);
    pthread_join(recv_thread, &thread_return);
    close(clientSock);
    return 0;
}

void* send_msg(void* arg)
{
    int sock = *((int*)arg);
    char name_msg[NAMESIZE + BUFSIZE];
    while (1) {
        fgets(msg, BUFSIZE, stdin);
        if (!strcmp(msg, "q")) {
            close(sock);
            exit(0);
        }
        sprintf(name_msg, "%s %s", name, msg);
        write(sock, name_msg, strlen(name_msg));
    }
    return NULL;
}

void* recv_msg(void* arg)
{
    int sock = *(int*)arg;
    char name_msg[NAMESIZE + BUFSIZE];
    int str_len;
    while (1) {
        str_len = read(sock, name_msg, NAMESIZE + BUFSIZE - 1);
        if (str_len == -1) {
            return (void*)-1;
        }
        name_msg[str_len] = 0;
        fputs(name_msg, stdout);
    }
    return NULL;
}