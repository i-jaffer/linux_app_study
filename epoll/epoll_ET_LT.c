#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include "wrap.h"

#define BUFMAX          12

void sys_error(char *str)
{
        perror(str);
        exit(-1);
}

void child_process(int fd)
{
        char ch = 'a';
        char buf[BUFMAX];
        int i=0;
        while(1) {
                for(i=0; i<(BUFMAX/2); i++)
                        buf[i] = ch;
                buf[i] = '\n';
                ch++;
                
                for( ; i<BUFMAX; i++)
                        buf[i] = ch;
                buf[i] = '\n';
                ch++;

                Write(fd, buf, BUFMAX);
                sleep(5);
        }
}

void parent_process(int fd)
{
        int ret = 0;
        int epfd = 0;
        char buf[BUFMAX];
        epfd = epoll_create(10);
        if(epfd == -1)
                sys_error("epoll_create error");

        struct epoll_event evt;
        //evt.events = EPOLLIN | EPOLLET;       /* ET 边沿触发 */
        evt.events = EPOLLIN;                   /* LT 水平触发(默认) */
        evt.data.fd = fd;
        ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &evt);
        if(ret == -1)
                sys_error("epoll_ctl error");

        struct epoll_event evts[1];
        int len = 0;
        while(1) {
                ret = epoll_wait(epfd, evts, 1, -1);
                if(ret == -1)
                        sys_error("epoll_wait");
                printf("\n\n************\n");
                len = Read(fd, buf, BUFMAX/2);
                Write(STDOUT_FILENO, buf, len);
        }
}

int main()
{
        int fd_pipe[2];
        int ret = 0;
        pid_t pid;

        ret = pipe(fd_pipe);
        if(ret == -1)
                sys_error("pipe error");
        
        pid = fork();
        if(pid == -1) {
                sys_error("fork error");
        } else if(pid == 0) {
                Close(fd_pipe[0]);      /* close pipe read */
                child_process(fd_pipe[1]);
        } else {
                Close(fd_pipe[1]);      /* close pipe write */
                parent_process(fd_pipe[0]);
        }
        
        return 0;
}
