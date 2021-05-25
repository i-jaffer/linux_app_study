#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <string.h>
#include "wrap.h"

#define PORT    8888
#define IP      "127.0.0.1"

struct connect_info_struct {
        int fd_client;
        int port;
        char ip_addr[20];
};

/* 
 * @brief: child thread: socket communicate
 * @note: receive client data, convert lowercase letters to uppercase letters, reture to client
 */
void *do_work(void *arg)
{
        struct connect_info_struct *conn_struct = arg;
        char buf[BUFSIZ];
        int ret = 0;

        printf("IP = %s Port: %d\n", conn_struct->ip_addr, conn_struct->port);
        while(1) {
                ret = Read(conn_struct->fd_client, buf, sizeof(buf));
                if(ret == 0) {
                        break;
                }
                for(int i=0; i<ret; i++) {
                        buf[i] = toupper(buf[i]);
                }
                write(STDOUT_FILENO, buf, ret);
                write(conn_struct->fd_client, buf, ret);
        }

        free(conn_struct);
        pthread_exit((void *)1);
}

int main()
{
        int ret = 0, i = 0;
        int fd_server, fd_client;

        /* create socket */
        fd_server = Socket(AF_INET, SOCK_STREAM, 0);
        
        /* bind ip and port message */
        struct sockaddr_in server_sockaddr;
        server_sockaddr.sin_family = AF_INET;
        server_sockaddr.sin_port = htons(PORT);
        ret = inet_pton(AF_INET, IP, &server_sockaddr.sin_addr.s_addr);
        if(ret == -1 || ret == 0) {
                fprintf(stderr,"inet_pton error: %s\n",strerror(ret));
                exit(-1);
        }
        socklen_t server_addrlen = sizeof(server_sockaddr);
        Bind(fd_server, (struct sockaddr *)&server_sockaddr, server_addrlen);
        /* set an upper limit on the number of connections to the server at one time */
        Listen(fd_server, 128);

        pthread_t tid;
        struct sockaddr_in client_sockaddr;
        socklen_t client_addrlen = sizeof(client_sockaddr);
        struct connect_info_struct *conn_info;
        while(1) {
                /* listen for client connections */
                socklen_t client_addrlen = sizeof(client_sockaddr);
                fd_client = Accept(fd_server, (struct sockaddr*)&client_sockaddr, &client_addrlen);

                /* request memory for the connection structure */
                conn_info = malloc(sizeof(struct connect_info_struct));
                if(conn_info == NULL) {
                        printf("malloc error, exit!\n");
                        exit(-1);
                }

                /* write date to the connection structure */
                conn_info->fd_client = fd_client;
                conn_info->port = ntohs(client_sockaddr.sin_port);
                inet_ntop(AF_INET, &client_sockaddr.sin_addr.s_addr, conn_info->ip_addr, sizeof(conn_info->ip_addr));

                /* create child thread to socket communication */
                pthread_create(&tid, NULL, do_work, conn_info);
                pthread_detach(tid);
        }
}

