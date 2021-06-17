#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT    8888

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
        
        struct sockaddr_in client_sockaddr;
        client_sockaddr.sin_family = AF_INET;
        client_sockaddr.sin_port = htons(PORT);
        //inet_aton("127.0.0.1", &(client_sockaddr.sin_addr));
        inet_pton(AF_INET, "127.0.0.1", &(client_sockaddr.sin_addr.s_addr));

        char buf[BUFSIZ];
        while(1) {
                fgets(buf, BUFSIZ, stdin);
                printf("stdin:%s\n",buf);

                ret = sendto(fd_client, buf, strlen(buf), 0, 
                             (struct sockaddr*)&client_sockaddr,
                             sizeof(client_sockaddr));
                if(ret == -1)
                        sys_error("sendto error");

                ret = recvfrom(fd_client, buf, BUFSIZ, 0, NULL, 0);     /*不关系对端信息*/
                if(ret == -1)
                        sys_error("recvfrom error");

                write(STDOUT_FILENO, buf, ret);
        }

        close(fd_client);

        return 0;
}
