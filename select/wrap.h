#ifndef __WRAP_H_
#define __WRAP_H_


int Socket(int domain, int type, int protocol);
int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int Listen(int sockfd, int backlog);
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
ssize_t Read(int fildes, void *buf, size_t count);
ssize_t Write(int fildes, const void *buf, size_t nbyte);
int Close(int fildes);
ssize_t Readn(int fildes, void *vptr, size_t n);
ssize_t Writen(int fildes, const void *vptr, size_t n);
ssize_t Readline(int fd, void *vptr, size_t maxlen);

#endif /*__WRAP_H_ */
