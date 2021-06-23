#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#define SERV_ADDR       "serv.socket"

void sys_error(char* str)
{
        perror(str);
        exit(-1);
}

int main(void)
{
        int ret = 0;
        int fd_server, fd_client;
        int len;

        fd_server = socket(AF_UNIX, SOCK_STREAM, 0);
        if(fd_server == -1)
                sys_error("socket error");

        struct sockaddr_un server_addr;
        server_addr.sun_family = AF_UNIX;
        strcpy(server_addr.sun_path, SERV_ADDR);

        len = offsetof(struct sockaddr_un, sun_path) + strlen(server_addr.sun_path); /* calculate server_addr total length */

        unlink(SERV_ADDR);              /* 确保bind之前的serv.sock文件不存在，bind会创建该文件 */
        ret = bind(fd_server, (struct sockaddr*)&server_addr, len);      /* 注意参数3不是sizeof(server_addr) */
        if(ret == -1)
                sys_error("bind error");

        listen(fd_server, 20);

        printf("...bind....\n");
        struct sockaddr_un client_addr;
        char buf[BUFSIZ];
        while(1) {
                len = sizeof(client_addr);
                fd_client = accept(fd_server, (struct sockaddr*)&client_addr, &len);
                if(fd_client == -1)
                        sys_error("accept error");

                /* 给client_addr.sun_path添加'\0'，方便使用printf打印 */
                len -= offsetof(struct sockaddr_un, sun_path);
                client_addr.sun_path[len] = '\0';
                printf("client bind filiname: %s\n", client_addr.sun_path);

                /* 小写转大写 回写 */
                int size = 0;
                while((size = read(fd_client, buf, sizeof(buf))) > 0) {
                        for(int i=0; i<size; i++)
                                buf[i] = toupper(buf[i]);
                        write(fd_client, buf, size);
                }
                close(fd_client);
        }
        close(fd_server);
        return 0;
}
