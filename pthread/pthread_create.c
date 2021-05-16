#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *pthread_func(void *arg)
{
        printf("In func: thread id = %lu, pid = %d\n",pthread_self(),getpid());
}

int main()
{
        pthread_t tid;
        int ret = 0;

        printf("In Main1: thread id = %lu, pid = %d\n",pthread_self(),getpid());
        ret = pthread_create(&tid,NULL,pthread_func,NULL);
        if(ret != 0) {
                printf("pthread_create error: %d\n",ret);
                exit(-1);
        }

        printf("In Main2: thread id = %lu, pid = %d\n",pthread_self(),getpid());
        sleep(1);
        
        return 0;
}
