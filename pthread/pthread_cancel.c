#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

void *pthread_func1(void *arg)
{
        printf("thread 1 exiting\n");
        pthread_exit((void *)111);
}

void *pthread_func2(void *arg)
{
        printf("thread 2 exiting\n");
        pthread_exit((void *)222);
}

void *pthread_func3(void *arg)
{
        while(1) {
                printf("thread 3: I'm alive!\n");
                sleep(1);
                /* pthread_cancel 需要在程序中有取消点(或者调用系统函数，由系统函数内部有取消点) */
        }
        pthread_exit((void *)333);
}

int main()
{
        pthread_t tid = 0;
        int ret = 0;
        void *retval = NULL;

        ret = pthread_create(&tid,NULL,pthread_func1,NULL);
        if(ret != 0) {
                fprintf(stderr,"pthread_create error:%s\n",strerror(ret));
                exit(-1);
        }
        pthread_join(tid,&retval);
        printf("thread 1 exit code is %d\n",(int)retval);

        ret = pthread_create(&tid,NULL,pthread_func2,NULL);
        if(ret != 0) {
                fprintf(stderr,"pthread_create error:%s\n",strerror(ret));
                exit(-1);
        }
        pthread_join(tid,&retval);
        printf("thread 2 exit code is %d\n",(int)retval);

        ret = pthread_create(&tid,NULL,pthread_func3,NULL);
        if(ret != 0) {
                fprintf(stderr,"pthread_create error:%s\n",strerror(ret));
                exit(-1);
        }
        pthread_cancel(tid);
        pthread_join(tid,&retval);
        printf("thread 3 exit code is %d\n",(int)retval);

        return 0;
}
