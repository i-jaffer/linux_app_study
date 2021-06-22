#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT            8888

void sys_error(char *str)
{
        perror(str);
        exit(-1);
}

int main(void)
{
        int ret = 0;
        int fd_client;

        fd_client = socket(AF_INET, SOCK_DGRAM, 0);
        if(fd_client == -1)
                sys_error("socket error");

        struct sockaddr_in client_addr;
        bzero(&client_addr, sizeof(client_addr));
        client_addr.sin_family = AF_INET;
        client_addr.sin_port = htons(PORT);
        inet_pton(AF_INET, "0.0.0.0", &(client_addr.sin_addr.s_addr));
        ret = bind(fd_client, (struct sockaddr*)&client_addr, sizeof(client_addr));
        if(ret == -1)
                sys_error("bind error");
        else
                printf("....bind ok....\n");

        char buf[BUFSIZ];
        int len = 0;
        while(1) {
                len = recvfrom(fd_client, buf, sizeof(buf), 0, NULL, 0);
                write(STDOUT_FILENO, buf, len);
                //printf("%s", buf);
        }
        
        return 0;
}
