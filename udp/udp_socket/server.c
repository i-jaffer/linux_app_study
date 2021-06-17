#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/types.h>
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
        int fd_server;
        fd_server = socket(AF_INET, SOCK_DGRAM, 0);
        if(fd_server == -1)
                sys_error("socket error");
        
        struct sockaddr_in server_sockaddr;
        server_sockaddr.sin_family = AF_INET;
        server_sockaddr.sin_port = htons(PORT);
        server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        socklen_t server_addrlen = sizeof(struct sockaddr_in);
        ret = bind(fd_server, (struct sockaddr *)&server_sockaddr, server_addrlen);
        if(ret == -1)
                sys_error("bind error");

        printf("Accepting connect ...\n");
        struct sockaddr_in client_sockaddr;
        socklen_t client_addrlen = sizeof(struct sockaddr_in);
        char buf[BUFSIZ];
        char str[BUFSIZ];
        while(1) {
                ret = recvfrom(fd_server, buf, sizeof(buf), 0, 
                         (struct sockaddr*)&client_sockaddr, &client_addrlen);
                if(ret == -1)
                        sys_error("recvfrom error");

                printf("receive from %s at PORT %d\n",
                       inet_ntop(AF_INET, &client_sockaddr.sin_addr, str, sizeof(str)),
                       ntohs(client_sockaddr.sin_port));

                for(int i=0; i<ret; i++)
                        buf[i] = toupper(buf[i]);

                ret = sendto(fd_server, buf, ret, 0, (struct sockaddr*)&client_sockaddr, client_addrlen);
                if(ret == -1)
                        sys_error("sendto error");
        }

        close(fd_server);

        return 0;
}
