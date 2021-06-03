#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>

static void sys_error(char *str)
{
        perror(str);
        exit(-1);
}

int Socket(int domain, int type, int protocol)
{
        int fd;
        fd = socket(domain, type, protocol);
        if(fd == -1)
                sys_error("socket error");
        return fd;
}

int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
        int ret = 0;
        ret = bind(sockfd, addr, addrlen);
        if(ret == -1)
                sys_error("bind error");
        return ret;
}

int Listen(int sockfd, int backlog)
{
        int ret = 0;
        ret = listen(sockfd, backlog);
        if(ret == -1)
                sys_error("listen error");
        return ret;
}

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
        int ret = 0;
again:
        ret = accept(sockfd, addr, addrlen);
        if(ret == -1) {
                if((errno == EINTR) || (errno == ECONNABORTED))
                        goto again;
                else 
                        sys_error("accept error");
        }
        return ret;
}

int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
        int ret = 0;
        ret = connect(sockfd, addr, addrlen);
        if(ret == -1)
                sys_error("connect error");
        return ret;
}

ssize_t Read(int fildes, void *buf, size_t count)
{
        ssize_t n;

again:
        n = read(fildes, buf, count);
        if((n == -1) && (errno == EINTR))
                goto again;

        return n;
}

ssize_t Write(int fildes, const void *buf, size_t nbyte)
{
        ssize_t ret = 0;

again:
        ret = write(fildes, buf, nbyte);
        if((ret == -1) && (errno == EINTR))
                goto again;
        
        return ret;
}

int Close(int fildes)
{
        int ret = 0;
        ret = close(fildes);
        if(ret == -1)
                sys_error("close error");
        return ret;
}

/*
 * @brief read n bytes of data
 * @param fildes: file descriptor 
 * @param vptr: the header address of thr receive data cache
 * @param n: the length of receive data
 * @retval success: the length of read data. failed: -1
 */
ssize_t Readn(int fildes, void *vptr, size_t n)
{
        ssize_t nread;
        size_t nbyte;
        unsigned char *ptr;
        ssize_t ret = 0;

        nbyte = n;
        ptr = vptr;

        while(nbyte > 0) {
                nread = read(fildes, ptr, nbyte);
                if(nread == -1) {
                        if(errno == EINTR)
                                nread = 0;
                        else {
                                ret = -1;
                                goto out;
                        }
                } else if(nread == 0)
                        break;
                nbyte -= nread;
                ptr += nread;
        }
        ret = n - nbyte;

out:
        return ret;
}

/*
 * @brief write n bytes of data
 * @param fildes: file descriptor 
 * @param vptr: the header address of thr send data cache
 * @param n: the length of send data
 * @retval success: the length of write data. failed: -1
 */
ssize_t Writen(int fildes, const void *vptr, size_t n)
{
        unsigned const char *ptr;
        size_t nbyte;
        ssize_t nwriten;
        ssize_t ret = 0;

        ptr = vptr;
        nbyte = n;

        while(nbyte > 0) {
                nwriten = write(fildes, ptr, nbyte);
                if(nwriten == -1) {
                        if(errno == EINTR)
                                nwriten = 0;
                        else {
                                ret = -1;
                                goto out;
                        }
                }

                nbyte -= nwriten;
                ptr += nwriten;
        }
        ret = n - nbyte;

out:
        return ret;
}

static ssize_t my_read(int fd,unsigned char *ptr)
{
        static int read_cnt = 0;
        static char *read_ptr;
        static char read_buf[100];
        ssize_t ret = 0;

        if(read_cnt <= 0) {
again:
                read_cnt = read(fd, read_buf, sizeof(read_buf));
                if(read_cnt == -1) {
                        if(errno == EINTR)
                                goto again;
                        else {
                                ret = -1;
                                goto out;
                        }
                } else if(read_cnt == 0)
                        return 0;

                read_ptr = read_buf;
        }

        read_cnt --;
        *ptr = *read_ptr ++;

out:
        return ret;
}

ssize_t Readline(int fd, void *vptr, size_t maxlen)
{
        ssize_t n, rc;
        unsigned char c, *ptr;
        ptr = vptr;

        for(n=1; n<maxlen; n++) {
                rc = my_read(fd, &c);
                if(rc == 1) {
                        *ptr++ = c;
                        if(c == '\n')
                                break;
                } else if(rc == 0) {
                        *ptr = 0;
                        return n-1;
                } else
                        return -1;
        }
        *ptr = 0;

        return n;
}

