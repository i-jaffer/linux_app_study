#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SERV_ADDR       "serv.socket"
#define CLIE_ADDR       "clie.socket"

void sys_error(char *str)
{
        perror(str);
        exit(-1);
}

int main(void)
{
        int ret = 0;
        int fd_client;
        int len = 0;

        fd_client = socket(AF_UNIX, SOCK_STREAM, 0);
        if(fd_client == -1)
                sys_error("socket error");

        /* 配置客户端信息结构体，并进行绑定 */
        struct sockaddr_un client_addr;
        bzero(&client_addr, sizeof(client_addr));
        client_addr.sun_family = AF_UNIX;
        strcpy(client_addr.sun_path, CLIE_ADDR);
        /* 计算客户端地址结构有效数据长度 */
        len = offsetof(struct sockaddr_un, sun_path) + strlen(client_addr.sun_path);
        unlink(CLIE_ADDR);
        ret = bind(fd_client, (struct sockaddr*)&client_addr, len);

        /* 配置服务器信息结构体，并进行连接 */
        struct sockaddr_un server_addr;
        bzero(&server_addr, sizeof(server_addr));
        server_addr.sun_family = AF_UNIX;
        strcpy(server_addr.sun_path, SERV_ADDR);
        /* 计算服务器地址结构有效数据长度 */
        len = offsetof(struct sockaddr_un, sun_path) + strlen(server_addr.sun_path);
        
        ret = connect(fd_client, (struct sockaddr*)&server_addr, len);
        if(ret == -1)
                sys_error("connect error");

        char buf[BUFSIZ];
        while(fgets(buf, sizeof(buf), stdin) != NULL) {
                write(fd_client, buf, strlen(buf));
                len = read(fd_client, buf, sizeof(buf));
                write(STDOUT_FILENO, buf, len);
        }

        close(fd_client);
        return 0;
}
