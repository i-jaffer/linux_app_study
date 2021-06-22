#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <net/if.h>

#define GROUP           "239.0.0.2"     /* 组播地址 */

#define SERVER_PORT     8888
#define CLIENT_PORT     9999

void sys_error(char* str)
{
        perror(str);
        exit(-1);
}

int main(int argc, char* argv[])
{
        int ret = 0;
        int fd_server;

        fd_server = socket(AF_INET, SOCK_DGRAM, 0);
        if(fd_server == -1)
                sys_error("socket error");

        /* 构造并绑定 server 地址 IP+端口 */
        struct sockaddr_in server_addr;
        bzero(&server_addr, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(SERVER_PORT);
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        ret = bind(fd_server, (struct sockaddr*)&server_addr, sizeof(server_addr));
        if(ret == -1)
                sys_error("bind error");

        struct ip_mreqn group;
        inet_pton(AF_INET, GROUP, &group.imr_multiaddr);        /* 设置组地址 */
        inet_pton(AF_INET, "0.0.0.0", &group.imr_address);      /* 设置本地IP */
        group.imr_ifindex = if_nametoindex("ens33");            /* 根据网卡名转化对应编号 编码查询命令 ip ad */

        setsockopt(fd_server, IPPROTO_IP, IP_MULTICAST_IF, &group, sizeof(group));      /* 设置组播权限 */

        /* 构造 client 地址 IP+端口 */
        struct sockaddr_in client_addr;
        bzero(&client_addr, sizeof(client_addr));
        client_addr.sin_family = AF_INET;
        client_addr.sin_port = htons(CLIENT_PORT);
        inet_pton(AF_INET, GROUP, &(client_addr.sin_addr.s_addr));

        int i = 0;
        char buf[BUFSIZ];
        while(1) {
                sprintf(buf, "multicast %dth message send...\n",i++);
                sendto(fd_server, buf, strlen(buf), 0,
                       (struct sockaddr*)&client_addr, sizeof(client_addr));
                sleep(1);
        }

        return 0;
}
