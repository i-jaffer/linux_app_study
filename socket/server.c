#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>

/* socket learn: (server)接收客户端发送的字符串，将其转化为大写，之后返回 */

#define PORT    6666
#define IP      "127.0.0.1"

void sys_error(char *str)
{
        perror(str);
        exit(-1);
}

int main()
{
        int ret = 0;
        int fd_server, fd_client;

        /* 1.建立套接字 */
        fd_server = socket(AF_INET, SOCK_STREAM, 0);
        if(fd_server == -1)
                sys_error("socket error");
        
        /* 2.绑定IP和端口号 */
        struct sockaddr_in sockaddr;
        sockaddr.sin_family = AF_INET;
        sockaddr.sin_port = htons(PORT);
        ret = inet_pton(AF_INET, IP, &sockaddr.sin_addr.s_addr);
        if(ret <= 0) {
                if(ret = 0) {
                        fprintf(stderr,"Not in presentation format");
                        exit(-1);
                } else {
                        sys_error("inet_pton error");
                }
        }
        //sockaddr.sin_addr.s_addr = (INADDR_ANY);
        ret = bind(fd_server, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
        if(ret == -1)
                sys_error("bind error");

        /* 3.设置最大同时发起连接数  tips:最大128 */
        ret = listen(fd_server, 128);
        if(ret == -1)
                sys_error("listen error");

        /* 4.阻塞等待客户端发起连接 */
        struct sockaddr_in sockaddr_client;
        socklen_t client_addr_len = sizeof(sockaddr_client);
        fd_client = accept(fd_server, (struct sockaddr *)&sockaddr_client, &client_addr_len);
        if(fd_client == -1)
                sys_error("accept error");

        /* 打印输出客户端信息 */
        unsigned char client_ip_buf[BUFSIZ];
        printf("Client IP:%s Client Port:%d\n",
               inet_ntop(AF_INET, &sockaddr_client.sin_addr.s_addr, client_ip_buf, sizeof(client_ip_buf)),
               ntohs(sockaddr_client.sin_port));

        /* 5.读取客户端数据 */
        char buf[1024];
        ret = read(fd_client, buf, sizeof(buf));
        if(ret == -1)
                sys_error("read error");
        for(int i=0; i<ret; i++) {
                buf[i] = toupper(buf[i]);
        }

        /* 6.写客户端数据 */
        ret = write(fd_client, buf, ret);
        if(ret == -1)
                sys_error("write error");

        /* 7.关闭接口 */
        close(fd_server);
        close(fd_client);

        return 0;
}
