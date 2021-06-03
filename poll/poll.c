#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include "wrap.h"

#define OPEN_MAX        1024
#define PORT            8888

int main()
{
        int ret = 0, nready = 0;
        int fd_listen;
        fd_listen = Socket(AF_INET, SOCK_STREAM, 0);

        struct sockaddr_in listen_sockaddr;
        listen_sockaddr.sin_family = AF_INET;
        listen_sockaddr.sin_port = htons(PORT);
        listen_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        socklen_t listen_addrlen = sizeof(struct sockaddr_in);
        Bind(fd_listen, (struct sockaddr *)&listen_sockaddr, listen_addrlen);

        Listen(fd_listen, 128);

        struct pollfd client[OPEN_MAX];
        client[0].fd = fd_listen;
        client[0].events = POLLIN;

        for(int i=1; i<OPEN_MAX; i++)
                client[i].fd = -1;

        int client_count = 0;   /* 记录客户端的数量个数 */

        while(1) {
                nready = poll(client, client_count+1, -1);
                if(nready == -1) {
                        perror("poll error");
                        exit(-1);
                }
                printf("nready = %d\n",nready);

                if(client[0].revents & POLLIN) {        /* 监听是否有客户端主动连接 */
                        int fd_client, i;
                        struct sockaddr_in client_sockaddr;
                        socklen_t client_socklen = sizeof(struct sockaddr);
                        fd_client = Accept(fd_listen, (struct sockaddr *)&client_sockaddr, &client_socklen);
                        if(fd_client == -1) {
                                perror("accept error");
                                exit(-1);
                        }
                        printf("accept success!\n");
                        printf("client IP:%s PORT:%d\n", 
                               inet_ntoa(client_sockaddr.sin_addr),
                               ntohs(client_sockaddr.sin_port));
                        
                        for(i=1; i<OPEN_MAX; i++) {
                                if(client[i].fd < 0) {
                                        client[i].fd = fd_client;
                                        client[i].events = POLLIN;
                                        break;
                                }
                        }

                        if(i == OPEN_MAX) {
                                printf("too many client!\n");
                                exit(-1);
                        }

                        client_count ++;
                        if(--nready == 0)
                                continue;
                }

                for(int i=1; i<(client_count+1); i++) {     /* 检测到client[]有数据就绪 */
                        if(client[i].fd == -1)
                               continue;
                        int n;
                        char buf[BUFSIZ];
                        if(client[i].revents & POLLIN) {
                                n = Read(client[i].fd, buf, sizeof(buf));
                                if(n < 0) {
                                        if(errno == ECONNRESET) {
                                                printf("client[%d] aborted connection\n",i);
                                                Close(client[i].fd);
                                                client[i].fd = -1;
                                        }
                                } else if(n == 0) {     /* 客户端关闭连接 */
                                        printf("client[%d] closed connection\n",i);
                                        close(client[i].fd);
                                        client[i].fd = -1;
                                } else {
                                        printf("Read: %s\n",buf);
                                        for(int j=0; j<n; j++)
                                                buf[j] = toupper(buf[j]);
                                        Write(client[i].fd, buf, n);
                                }

                                if(--nready == 0)
                                        break;
                        }
                }
        }

        return 0;
}
