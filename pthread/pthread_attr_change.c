#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define SIZE    0x100000000

void *pthread_func(void *arg)
{
        while(1)
                sleep(1);
        pthread_exit((void *)1);
}

int main()
{
        pthread_t tid;
        int ret = 0;
        pthread_attr_t attr;
        void *stackaddr;
        size_t stacksize;
        int detachstate;

        ret = pthread_attr_init(&attr);
        if(ret != 0) {
                fprintf(stderr,"pthread_attr_init error:%s\n",strerror(ret));
                exit(-1);
        }
        pthread_attr_getstack(&attr, &stackaddr, &stacksize);
        printf("thread stack size : %ld\n",stacksize);
        pthread_attr_getdetachstate(&attr, &detachstate);
        if(detachstate == PTHREAD_CREATE_DETACHED)
                printf("thread detached!\n");
        else
                printf("thread joinable\n");

        /* 设置线程分离属性 */
        pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

        for(int i=0; ;i++) {
                /* 在堆上申请内存，制定栈的起始地址和栈大小 */
                stacksize = SIZE;
                stackaddr = (void *)malloc(stacksize);
                if(stackaddr == NULL) {
                        perror("malloc error:");
                        printf("pthread count: %d\n",i);
                        exit(-1);
                }
                ret = pthread_attr_setstack(&attr, stackaddr, stacksize);
                if(ret != 0) {
                        fprintf(stderr,"pthread_attr_setstack error:%s\n",strerror(ret));
                        exit(-1);
                }

                ret = pthread_create(&tid,&attr,pthread_func,NULL);
                if(ret != 0) {
                        fprintf(stderr,"pthread_create error:%s\n",strerror(ret));
                        printf("pthread count: %d\n",i);
                        exit(-1);
                }
        }

        //free(satckaddr);
        pthread_attr_destroy(&attr);

        return 0;
}
