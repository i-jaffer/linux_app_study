#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define SERVER_IP       "127.0.0.1"
#define PORT            6666

void sys_error(char *str)
{
        perror(str);
        exit(-1);
}

int main(int argc, char *argv[])
{
        int ret = 0;

        if(argc < 2) {
                printf("./a.out string\n");
                exit(-1);
        }

        /* 1.创建socket连接 */
        int fd_client;
        fd_client = socket(AF_INET, SOCK_STREAM, 0);
        if(fd_client == -1)
                sys_error("socket error");
        
        /* 可使用系统默认分配ip地址和端口,所以无需使用bind配置 */
#if 0
        /* 2.绑定IP和端口号 */
        struct sockaddr_in sockaddr;
        sockaddr.sin_family = AF_INET;
        sockaddr.sin_port = htons(1314);
        ret = inet_pton(AF_INET, "127.1.2.11", &sockaddr.sin_addr.s_addr);
        if(ret <= 0) {
                if(ret = 0) {
                        fprintf(stderr,"Not in presentation format");
                        exit(-1);
                } else {
                        sys_error("inet_pton error");
                }
        }
        //sockaddr.sin_addr.s_addr = (INADDR_ANY);
        ret = bind(fd_client, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
        if(ret == -1)
                sys_error("bind error");

#endif

        /* 2.与服务器建立连接 */
        struct sockaddr_in sockaddr_client;
        sockaddr_client.sin_family = AF_INET;
        sockaddr_client.sin_port = htons(PORT);
        inet_pton(AF_INET, SERVER_IP, &sockaddr_client.sin_addr.s_addr);
        ret = connect(fd_client, (struct sockaddr *)&sockaddr_client, sizeof(sockaddr_client));
        if(ret == -1)
                sys_error("connect error");

        /* 3.往服务器写入数据 */
        ret = write(fd_client, argv[1], strlen(argv[1])+1);
        if(ret == -1)
                sys_error("write error");

        /* 4.获取服务器返回的数据 */
        char buf[1024];
        ret = read(fd_client, buf, sizeof(buf));
        if(ret == -1)
                sys_error("read error");
        printf("read : %s\n",buf);

        /* 5.关闭接口 */
        close(fd_client);
        
        return 0;
}
