#ifndef __FIFO_H_
#define __FIFO_H_

#define QUEUE_SIZE      10

typedef struct fifo_stuct {
        int *queue;
        int size;
        int index;
        int vaild_num;
}fifo_struct_typedef;


int queue_init(fifo_struct_typedef *fifo_struct, int size);
int queue_deinit(fifo_struct_typedef *fifo_struct);
int queue_in(fifo_struct_typedef *fifo_struct, int data);
int queue_out(fifo_struct_typedef *fifo_struct);

#endif
