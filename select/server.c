#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/types.h>
#include <ctype.h>
#include "wrap.h"

#define PORT    8888

void sys_error(char *str)
{
        perror(str);
        exit(-1);
}

int main()
{
        int fd_max, fd_listen, fd_connect, fd_socked;
        struct sockaddr_in listen_sockaddr, conn_sockaddr;
        socklen_t listen_addrlen, conn_addrlen;

        fd_set all_set, ret_set;
        /* 自定义数组，避免遍历1024文件描述符,FD_SETSIZE默认大小为1024 */
        int nready, client[FD_SETSIZE];
        int maxi = -1, i = 0;
        char buf[BUFSIZ], str[INET_ADDRSTRLEN];         /* #define INET_ADDRSTRLEN 16 */

        fd_listen = Socket(AF_INET, SOCK_STREAM, 0);
        listen_sockaddr.sin_family = AF_INET;
        listen_sockaddr.sin_port = htons(PORT);
        listen_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        listen_addrlen = sizeof(struct sockaddr_in);
        Bind(fd_listen, (struct sockaddr *)&listen_sockaddr, listen_addrlen);
        Listen(fd_listen, 128);

        for(int i=0; i<FD_SETSIZE; i++)         /* 使用-1初始化client[] */
                client[i] = -1;

        fd_max = fd_listen;
        FD_ZERO(&all_set);
        FD_SET(fd_listen, &all_set);

        while(1) {
                ret_set = all_set;      /* 由于调用select all_set的值会改变，所以需要缓存，使用ret_set */
                nready = select(fd_max+1, &ret_set, NULL, NULL, NULL);
                if(nready == -1)
                        sys_error("select error");

                if(FD_ISSET(fd_listen, &ret_set)) {     /* 说明有新的客户端连接请求 */
                        conn_addrlen = sizeof(struct sockaddr);
                        fd_connect = Accept(fd_listen, (struct sockaddr *)&conn_sockaddr, &conn_addrlen);
                        printf("receive from %s at PORT %d\n", 
                               inet_ntop(AF_INET, &conn_sockaddr.sin_addr.s_addr, str, sizeof(str)),
                               ntohs(conn_sockaddr.sin_port));
                        
                        /* 将client文件指针添加进入client[]数组 */
                        for(i=0; i<FD_SETSIZE; i++) {
                                if(client[i] < 0) {     /* 寻找client[]内没有使用的位置 */
                                        client[i] = fd_connect;
                                        break;
                                }
                        }
                        if(i == FD_SETSIZE) {           /* 客户端超过上限处理 */
                                printf("error:too many client!\n");
                                exit(-1);
                        }

                        FD_SET(fd_connect, &all_set);   /* 将新的文件描述符添加进入readset */
                        if(fd_connect > fd_max)         /* 修改最大的文件描述符 */
                                fd_max = fd_connect;

                        if(i > maxi)                    /* 保证maxi总是client[]最后一个元素下标 */
                                maxi = i;

                        if(--nready == 0)
                                continue;
                }

                for(int i=0; i<=maxi; i++) {            /* 检测哪一个client[]有数据就绪 */
                        fd_socked = client[i];

                        if(fd_socked < 0)
                                continue;
                        if(FD_ISSET(fd_socked, &ret_set)) {
                                int n = Read(fd_socked, buf, sizeof(buf));
                                if(n == 0) {            /* 客户端关闭连接，服务端对应关闭 */
                                        Close(fd_socked);
                                        FD_CLR(fd_socked, &all_set);
                                        client[i] = -1; /* 解除监控 */
                                } else if(n > 0) {
                                        for(int j=0; j<n; j++)
                                                buf[j] = toupper(buf[j]);
                                        Write(fd_socked, buf, n);
                                }
                                if(--nready == 0)       /* nready记录了存在数据的client个数 */
                                        break;
                        }
                }
        }
        Close(fd_listen);

        return 0;
}
