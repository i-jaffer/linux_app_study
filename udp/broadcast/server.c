#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <strings.h>
#include <arpa/inet.h>
#include <string.h>

#define SERVER_PORT     8888
#define CLIENT_PORT     8888
#define BROADCAST_IP    "192.168.43.255"

void sys_error(char *str)
{
        perror(str);
        exit(-1);
}

int main(int argvc, char *argv[])
{
        int ret = 0;
        int fd_server;
        fd_server = socket(AF_INET, SOCK_DGRAM, 0);
        if(fd_server == -1)
                sys_error("socket error");

        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htonl(SERVER_PORT);
        server_addr.sin_addr.s_addr = htons(INADDR_ANY);
        ret = bind(fd_server, (struct sockaddr*)&server_addr, sizeof(server_addr));
        if(ret == -1)
                sys_error("bind error");

        /* 设置允许广播 */
        int flag = 1;
        ret = setsockopt(fd_server, SOL_SOCKET, SO_BROADCAST, &flag, sizeof(flag));
        if(ret == -1)
                sys_error("setsockopt error");

        /* 构造 client 地址ip+端口 */
        struct sockaddr_in client_addr;
        bzero(&client_addr,sizeof(client_addr));
        client_addr.sin_family = AF_INET;
        client_addr.sin_port = htons(CLIENT_PORT);
        inet_pton(AF_INET, BROADCAST_IP, &(client_addr.sin_addr.s_addr));

        int i = 0;
        char buf[BUFSIZ];
        while(1) {
                sprintf(buf, "Drink %d glasses of water\n", i++);
                sendto(fd_server, buf, strlen(buf), 0,
                       (struct sockaddr*)&client_addr, sizeof(client_addr));
                sleep(1);
        }

        return 0;
}
