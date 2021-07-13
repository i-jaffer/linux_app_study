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

/*
 * @brief register client
 * @param id:client id pointer
 * @retval 0:sucess -1:failed
 */
int register_client(char *id)
{
        int ret = 0;

        if(id == NULL) {
                ret = -1;
                goto out;
        }

        /* 检查是否已经注册 */
        for(int i=0; i<100; i++) {
                if(client_msg_list[i].status == 0) {
                        if(strncmp(client_msg_list[i].id, id, 4) == 0) {
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
                        strcpy(client_msg_list[i].id, id);
                        printf("register client success!\n");
                        ret = 0;
                        goto out;
                }
        }

        /* space full */
        ret = -1;
out:
        return 0;
}

/*
 * @brief unregister client
 * @param *id:client id pointer
 * @retval 0:success -1:failed
 */
int unregister_client(char *id)
{
        int ret = 0;
        if(id == NULL) {
                ret = -1;
                goto out;
        }

        for(int i=0; i<100; i++) {
                if(client_msg_list[i].status == 0) {
                        if(strncmp(client_msg_list[i].id, id, 4) == 0) {
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

/*
 * @brief parse data packet by client
 * @param src:data sender
 * @param dest:data receiver
 * @param data:the data content
 * @retval 0:success -1:failed
 */
int parse_packet_by_client(char *src, char *dest, char *data)
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
                printf("strlen buf:%ld\n\n", strlen(buf));
                if(strlen(buf) > 4) {
                        switch(buf[0]) {
                        case '0':
                                ret = register_client(&buf[1]);
                                if(ret != 0)
                                        printf("register client failed!\n");
                                break;
                        case '1':
                                ret = unregister_client(&buf[1]);
                                if(ret != 0)
                                        printf("unregister cient failed!\n");
                                break;
                        case '2':
                                ret = parse_packet_by_client(&buf[1], &buf[5], &buf[9]);
                                if(ret != 0)
                                        printf("sendmsg_by_client failed!\n");
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
