#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include "wrap.h"

#define PORT    8888

void child_progress_recycling(int signo)
{
        int ret = 0;
        do {
                ret  = waitpid(0, NULL, WNOHANG);
        }while(ret > 0);
        return ;
}

int main()
{
        int fd_server = 0;
        int ret = 0;
        
        fd_server = Socket(AF_INET, SOCK_STREAM, 0);

        struct sockaddr_in server_addr;
        socklen_t server_addrlen = sizeof(server_addr);
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(PORT);
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        //inet_pton(AF_INET, "127.0.0.3", &server_addr.sin_addr.s_addr);
        Bind(fd_server, (struct sockaddr *)&server_addr, server_addrlen);

        Listen(fd_server, 128);

        pid_t pid;
        int fd_client = 0;
        struct sockaddr_in client_addr;
        socklen_t client_addrlen = sizeof(client_addr);
        while(1) {
                fd_client = Accept(fd_server, (struct sockaddr*)&client_addr, &client_addrlen);
                char temp_buf[BUFSIZ];
                printf("IP:%s Port:%d\n", inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, temp_buf,sizeof(temp_buf)),
                       ntohs(client_addr.sin_port));

                /* create child progress to socket communication */
                pid = fork();
                if(pid == -1) {
                        perror("fork error");
                        exit(-1);
                } else if(pid == 0) {
                        close(fd_server);
                        break;
                }
                else {
                        close(fd_client);
                        signal(SIGCHLD, child_progress_recycling);
                }
        }

        /* child process: complete socket communication */
        if(pid == 0) {
                unsigned char buf[BUFSIZ];
                ret = Read(fd_client, buf, sizeof(buf));
                if(ret == -1) {
                        perror("read error");
                        exit(-1);
                } else if(ret == 0) {
                        close(fd_client);
                } else {
                        for(int i=0; i<ret; i++)
                                buf[i] = toupper(buf[i]);
                        write(fd_client, buf, ret);
                        write(STDOUT_FILENO, buf, ret);
                }
        }

        return 0;
}
