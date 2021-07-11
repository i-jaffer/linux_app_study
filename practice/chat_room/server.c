#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#define UNRECE_FIFO "universal_recefifo"

typedef struct client_msg_struct {
        char status;
        char id[4];
        int online_timestamp;
}client_msg_typedef;
client_msg_typedef client_msg_list[100] = {0};

void sys_error(char *str)
{
        perror(str);
        exit(-1);
}

int client_register(char *msg)
{
        int ret = 0;

        if(msg == NULL) {
                ret = -1;
                goto out;
        }

        /* 检查是否已经注册 */
        for(int i=0; i<100; i++) {
                if(client_msg_list[i].status == 0) {
                        if(strncmp(client_msg_list[i].id, msg, 4) == 0) {
                                ret = 1;
                                printf("This client has logged in!\n");
                                goto out;
                        }
                }
        }

        /* register client */
        for(int i=0; i<100; i++) {
                if(client_msg_list[i].status == 0) {
                        client_msg_list[i].status = 1;
                        strcpy(client_msg_list[i].id, msg);
                        ret = 0;
                        printf("register client success!!\n");
                        goto out;
                }
        }

        /* space full */
        ret = -1;
out:
        return 0;
}

int client_unregister(char *msg)
{
        int ret = 0;
        if(msg == NULL) {
                ret = -1;
                goto out;
        }

        for(int i=0; i<100; i++) {
                if(client_msg_list[i].status == 0) {
                        if(strncmp(client_msg_list[i].id, msg, 4) == 0) {
                                memset(&(client_msg_list[i]), 0, sizeof(client_msg_list[i]));
                                ret = 0;
                                printf("client unregister success!\n");
                                goto out;
                        }
                }
        }

out:
        return ret;
}

int client_sendmsg(char *src, char *dest, char *data)
{
        int ret = 0;

        printf("src:%s dest:%s\n", src, dest);
        printf("data:%s\n", data);

        return ret;
}

int main()
{
        memset(client_msg_list, 0, sizeof(client_msg_list));

        int ret = 0;
        if(access(UNRECE_FIFO, F_OK) == -1) {
                ret = mkfifo(UNRECE_FIFO, 0666);
                if(ret == -1)
                        sys_error("make fifo error");
        }

        int fd = 0;
        fd = open(UNRECE_FIFO, O_RDONLY);
        if(fd == -1)
                sys_error("open receive fifo error");

        char buf[BUFSIZ];
        memset(buf, 0, sizeof(buf));

        while(1) {
                ret = read(fd, buf, sizeof(buf));
                if(ret == -1)
                        sys_error("read error");

                printf("readbuf: %s\n",buf);
                if(strlen(buf) > 4) {
                        switch(buf[0]) {
                        case 0:
                                client_register(&buf[1]);
                                break;
                        case 1:
                                client_sendmsg(&buf[1], &buf[5], &buf[9]);
                                break;
                        case 2:
                                client_unregister(&buf[1]);
                                break;
                        default:
                                break;
                        }
                }
                sleep(1);
        }

        close(fd);

        return 0;
}
