#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "fifo.h"

fifo_struct_typedef fifo;

void sys_error(char *str)
{
        printf("%s",str);
        exit(-1);
}

int main()
{
        int ret = 0, data = 0;
        ret = queue_init(&fifo, 5);
        if(ret == -1)
                sys_error("queue init error!");

        for(int i=0;i<4;i++) {
                data = rand()%10;
                ret = queue_in(&fifo,data);
                if(ret == -1) 
                        sys_error("queue in error\n");
                printf("queue in data:%d\n",data);
                sleep(1);
        }

        for(int i=0;i<4;i++) {
                data = queue_out(&fifo);
                if(data == -1)
                        sys_error("queue out error!\n");
                printf("queue out data:%d\n",data);
                sleep(1);
        }

        return 0;
}
