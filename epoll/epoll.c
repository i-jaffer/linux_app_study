#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <ctype.h>
#include "wrap.h"

#define PORT            8888

void sys_error(char *str)
{
        perror(str);
        exit(-1);
}

int main(void)
{
        int ret = 0;
        int fd_listen;
        fd_listen = Socket(AF_INET, SOCK_STREAM, 0);
        
        struct sockaddr_in listen_sockaddr;
        listen_sockaddr.sin_family = AF_INET;
        listen_sockaddr.sin_port = htons(PORT);
        listen_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        socklen_t listen_addrlen;
        listen_addrlen = sizeof(struct sockaddr_in);
        Bind(fd_listen, (struct sockaddr *)&listen_sockaddr, listen_addrlen);

        Listen(fd_listen, 128);

        int epfd;
        epfd = epoll_create(10);
        if(epfd == -1)
                sys_error("epoll_create error");

        struct epoll_event evt;
        evt.events = EPOLLIN;
        evt.data.fd = fd_listen;
        ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd_listen, &evt);
        if(ret == -1)
                sys_error("epoll_ctl error");

        int nready = 0;
        struct epoll_event evts[10];
        struct sockaddr_in client_sockaddr;
        socklen_t client_addrlen;
        int fd_client;
        char buf[BUFSIZ];
        while(1) {
                nready = epoll_wait(epfd, evts, 10, -1);        /* 等待有事件发生 */
                if(nready == -1)
                        sys_error("epoll_wait error");

                for(int i=0; i<nready; i++) {
                        if(!(evts[i].events & EPOLLIN))         /* 判断是否是读事件 */
                                continue;

                        if(evts[i].data.fd == fd_listen) {      /* 有客户端发起连接 */
                                fd_client = Accept(fd_listen, (struct sockaddr *)&client_sockaddr, &client_addrlen);
                                printf("Receive from %s at Port:%d\n",
                                       inet_ntoa(client_sockaddr.sin_addr),
                                       ntohs(client_sockaddr.sin_port));
                                evt.events = EPOLLIN;
                                evt.data.fd = fd_client;
                                ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd_client, &evt);
                                if(ret == -1)
                                        sys_error("epoll_ctl error");
                        } else {                                /* 有客户端产生IN事件 */
                                int n = 0;
                                n = Read(evts[i].data.fd, buf, sizeof(buf));
                                if(n == -1) {
                                        printf("Read error\n");
                                        ret = epoll_ctl(epfd, EPOLL_CTL_DEL, evts[i].data.fd, NULL);
                                        if(ret == -1)
                                                sys_error("epoll_ctl error");
                                        Close(evts[i].data.fd);
                                } else if(n == 0) {
                                        ret = epoll_ctl(epfd, EPOLL_CTL_DEL, evts[i].data.fd, NULL);
                                        if(ret == -1)
                                                sys_error("epoll_ctl error");
                                        Close(evts[i].data.fd);
                                        printf("client[%d] closed connection\n", evts[i].data.fd);
                                } else {
                                        for(int j=0; j<n; j++)
                                                buf[j] = toupper(buf[j]);
                                        Write(evts[i].data.fd, buf, n);
                                        Write(STDOUT_FILENO, buf, n);
                                }
                        }
                }
        }

        return 0;
}
