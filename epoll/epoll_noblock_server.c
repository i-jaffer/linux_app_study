#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "wrap.h"

#define PORT    8888
#define MAXSIZ  16

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
        socklen_t listen_addrlen = sizeof(struct sockaddr_in);
        Bind(fd_listen, (struct sockaddr *)&listen_sockaddr, listen_addrlen);

        Listen(fd_listen, 128);

        struct sockaddr_in conn_sockaddr;
        socklen_t conn_addrlen;
        int fd_conn;
        fd_conn = Accept(fd_listen, (struct sockaddr *)&conn_sockaddr, &conn_addrlen);

        printf("Receive %s at Port %d\n",
               inet_ntoa(conn_sockaddr.sin_addr),
               ntohs(conn_sockaddr.sin_port));

        /* 修改connfd为非阻塞 */
        int flag = 0;
        flag = fcntl(fd_conn, F_GETFL);
        flag |= O_NONBLOCK;
        fcntl(fd_conn, F_SETFL, flag);

        int epfd_conn;
        epfd_conn = epoll_create(10);
        if(epfd_conn == -1)
                sys_error("epoll_create error");

        struct epoll_event evt;
        evt.events = EPOLLIN | EPOLLET;         /* 设置为边沿触发模式 */
        evt.data.fd = fd_conn;
        ret = epoll_ctl(epfd_conn, EPOLL_CTL_ADD, fd_conn, &evt);
        if(ret == -1)
                sys_error("epoll_ctl error");

        char buf[MAXSIZ];
        int len = 0;
        while(1) {
                struct epoll_event evts[10];
                ret = epoll_wait(epfd_conn, evts, 10, -1);
                if(ret == -1)
                        sys_error("epoll_wait error");

                printf("\n====================\n");
                if(evts[0].data.fd == fd_conn) {
                        do {
                                len = Read(fd_conn, buf, MAXSIZ/2);
                                Write(STDOUT_FILENO, buf, len);
                        } while(len>0);
                }
        }
        close(fd_conn);

        return 0;
}
