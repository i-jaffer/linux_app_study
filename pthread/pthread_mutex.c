#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t mutex;

void *pthread_func(void *arg)
{
        while(1) {
                pthread_mutex_lock(&mutex);
                printf("hello ");
                sleep(rand()%3);        //模拟子线程长时间占用cpu
                printf("word!\n");
                pthread_mutex_unlock(&mutex);

                sleep(rand()%3);
        }
        pthread_exit((void *)1);
}

int main()
{
        pthread_t tid;
        int ret = 0;

        ret = pthread_mutex_init(&mutex,NULL);
        if(ret != 0) {
                fprintf(stderr,"pthread_mutex_init error:%s\n",strerror(ret));
                exit(-1);
        }

        ret = pthread_create(&tid, NULL, pthread_func, NULL);
        if(ret != 0) {
                fprintf(stderr,"pthread_create error:%s\n",strerror(ret));
                exit(-1);
        }

        while(1) {
                pthread_mutex_lock(&mutex);
                printf("Hello ");
                sleep(rand()%3);
                printf("Word!\n");
                pthread_mutex_unlock(&mutex);

                sleep(rand()%3);
        }
        pthread_join(tid, NULL);
        pthread_mutex_destroy(&mutex);

        return 0;
}
