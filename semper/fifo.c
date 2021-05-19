#include <stdio.h>
#include <stdlib.h>
#include "fifo.h"

int queue_init(fifo_struct_typedef *fifo_struct, int size)
{
        fifo_struct->queue = (int *)malloc(size);
        fifo_struct->size = size;
        fifo_struct->index = 0;
        fifo_struct->vaild_num = 0;

        if(fifo_struct->queue == NULL)
                return -1;
        else
                return 0;
}

int queue_deinit(fifo_struct_typedef *fifo_struct)
{
        int ret = 0;
        free(fifo_struct->queue);
        fifo_struct->size = 0;
        fifo_struct->index = 0;
        fifo_struct->vaild_num = 0;

        return 0;
}

int queue_in(fifo_struct_typedef *fifo_struct, int data)
{
        int ret = 0;
        if(fifo_struct == NULL) {
                ret = -1;
                goto out;
        }
        if(fifo_struct->queue == NULL) {
                ret = -1;
                goto out;
        }

        if(fifo_struct->vaild_num == fifo_struct->size) {
                ret = -1;
                goto out;
        }

        fifo_struct->queue[fifo_struct->index] = data;
        fifo_struct->vaild_num ++;
        fifo_struct->index = (++(fifo_struct->index)) % (fifo_struct->size);

out:
        return ret;
}

int queue_out(fifo_struct_typedef *fifo_struct)
{
        int ret = 0;
        int vaild_index = 0;
        if(fifo_struct == NULL) {
                ret = -1;
                goto out;
        }
        if(fifo_struct->queue == NULL) {
                ret = -1;
                goto out;
        }

        if(fifo_struct->vaild_num ==  0) {
                ret = -1;
                goto out;
        }

        vaild_index = (fifo_struct->index + fifo_struct->size - fifo_struct->vaild_num) % 
                (fifo_struct->size);
        ret = fifo_struct->queue[vaild_index];
        fifo_struct->queue[vaild_index] = 0;
        fifo_struct->vaild_num --;

out:
        return ret;
}


