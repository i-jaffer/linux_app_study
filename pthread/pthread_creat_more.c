#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

void *pthread_func(void *arg)
{
        int i = (int)arg;
        sleep(i);
        printf("%dth thread: thread id %lu, pid %d\n",i+1,pthread_self(),getpid());
}

int main()
{
        pthread_t tid = 0;
        int ret = 0;
        for(int i=0; i<5; i++) {
                ret = pthread_create(&tid,NULL,pthread_func,(void *)i);
                if(ret != 0) {
                        fprintf(stderr,"pthread_creat error:%s\n",strerror(ret));
                        exit(-1);
                }
        }
        pthread_exit(NULL);
        return 0;
}
