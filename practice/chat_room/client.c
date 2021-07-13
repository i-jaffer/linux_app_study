#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define UNRECE_FIFO     "universal_recefifo"

typedef struct client_msg_struct {
        char src_id[4];
        char dest_id[4];
        char data[100];
}client_msg_typedef;

void sys_error(char *str)
{
        perror(str);
        exit(-1);
}

/*
 * @brief client init
 * @param *client: client message struct
 * @param *dest_id: data receiver id
 * @retval 0:success -1:failed
 */
int client_init(client_msg_typedef *client, char *dest_id)
{
        int ret = 0;
        if(client == NULL || dest_id == NULL) {
                ret = -1;
                goto out;
        }

        memset(client, 0, sizeof(client));

        for(int i=0; i<4; i++) {
                client->src_id[i] = rand() % 256;
                client->dest_id[i] = dest_id[i];
        }
        printf("src id:%d.%d.%d.%d\n",
               client->src_id[0],
               client->src_id[1],
               client->src_id[2],
               client->src_id[3]);
        printf("dest id:%d.%d.%d.%d\n",
               client->dest_id[0],
               client->dest_id[1],
               client->dest_id[2],
               client->dest_id[3]);

out:
        return ret;
}

/*
 * @brief register client to server
 * @param *client: client message struct
 * @param fd_fifo: server file specified
 * @retval 0:success -1:failed
 */
int client_register(client_msg_typedef *client, int fd_fifo)
{
        int ret = 0;
        char buf[100];

        buf[0] = '0';
        strncpy(&buf[1], client->src_id, 4);
        strncpy(&buf[5], client->dest_id, 4);

        ret = write(fd_fifo, buf, 9);
        if(ret == -1)
                sys_error("client_register write error");

        return ret;
}

/*
 * @brief unregister client to server
 * @param *client: client message struct
 * @param fd_fifo: server file specified
 * @retval 0:success -1:failed
 */
int client_unregister(client_msg_typedef *client, int fd_fifo)
{
        int ret = 0;
        char buf[100];

        buf[0] = '1';
        strncpy(&buf[1], client->src_id, 4);
        strncpy(&buf[5], client->dest_id, 4);

        ret = write(fd_fifo, buf, 9);
        if(ret == -1)
                sys_error("client_unregister write error");

        return ret;
}

int main()
{
        int fd_txfifo, fd_rxfifo;
        client_msg_typedef client_msg;

        fd_txfifo = open(UNRECE_FIFO, O_WRONLY);
        if(fd_txfifo == -1) {
                printf("please run server before client!\n");
                sys_error("open UNRECR_FIFO error");
        }

        printf("========== client launch ========\n");

        char destid[4] = {'A','B','C','D'};
        client_init(&client_msg, destid);
        client_register(&client_msg, fd_txfifo);

        while(1);

        //client_unregister(&client_msg, fd_txfifo);
        close(fd_txfifo);
        
        return 0;
}
