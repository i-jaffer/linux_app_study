#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include "wrap.h"

#define SERVER_PORT     8888
#define SERVER_IP       "127.0.0.1"

#define CLIENT_PORT     6666

#define MAXSIZE         16

void sys_error(char *str)
{
        perror(str);
        exit(-1);
}

int main(void)
{
        int ret = 0;
        int fd_client;
        fd_client = Socket(AF_INET, SOCK_STREAM, 0);

#if 1
        struct sockaddr_in client_sockaddr;
        client_sockaddr.sin_family = AF_INET;
        client_sockaddr.sin_port = htons(CLIENT_PORT);
        client_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        socklen_t client_socklen = sizeof(struct sockaddr_in);
        Bind(fd_client, (struct sockaddr*)&client_sockaddr, client_socklen);
#endif

        struct sockaddr_in server_sockaddr;
        server_sockaddr.sin_family = AF_INET;
        server_sockaddr.sin_port = htons(SERVER_PORT);
        inet_pton(AF_INET, SERVER_IP, &(server_sockaddr.sin_addr.s_addr));
        socklen_t server_socklen = sizeof(struct sockaddr);
        Connect(fd_client, (struct sockaddr*)&server_sockaddr, server_socklen);

        char buf[MAXSIZE];
        char ch = 'a';
        int i = 0;
        while(1) {
                for(i=0; i<MAXSIZE/2; i++)
                        buf[i] = ch;
                buf[i-1] = '\n';
                ch ++;
                
                for( ; i<MAXSIZE; i++)
                        buf[i] = ch;
                buf[i-1] = '\n';
                ch ++;

                Write(fd_client, buf, MAXSIZE);
                sleep(3);
        }
        
        return 0;
}
