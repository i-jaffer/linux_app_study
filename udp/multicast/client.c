#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <net/if.h>

#define GROUP           "239.0.0.2"
#define CLIENT_PORT     9999

void sys_error(char *str)
{
        perror(str);
        exit(-1);
}

int main(int argc, char* argv[])
{
        int ret = 0;
        int fd_client;
        fd_client = socket(AF_INET, SOCK_DGRAM, 0);
        if(fd_client == -1)
                sys_error("socket error");

        struct sockaddr_in client_addr;
        bzero(&client_addr, sizeof(client_addr));
        client_addr.sin_family = AF_INET;
        client_addr.sin_port = htons(CLIENT_PORT);
        client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        ret = bind(fd_client, (struct sockaddr*)&client_addr, sizeof(client_addr));
        if(ret == -1)
                sys_error("bind error");

        struct ip_mreqn group;
        inet_pton(AF_INET, GROUP, &group.imr_multiaddr);        /* 设置组播组地址 */
        inet_pton(AF_INET, "0.0.0.0", &group.imr_address);      /* 使用本地任意IP添加到组播组 */
        group.imr_ifindex = if_nametoindex("eth33");            /* 网卡名转化为编号 编号查询 ip ad */

        setsockopt(fd_client, IPPROTO_IP, IP_ADD_MEMBERSHIP, &group, sizeof(group)); /* 设置client加入到组播组 */

        int len = 0;
        char buf[BUFSIZ];
        while(1) {
                len = recvfrom(fd_client, buf, BUFSIZ, 0, NULL, 0);
                write(STDOUT_FILENO, buf, len);
        }

        close(fd_client);
        return 0;
}
