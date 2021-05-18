#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t mutex1;
pthread_mutex_t mutex2;

void *pthread_func(void *arg)
{
        int ret = 0;
        while(1) {
                pthread_mutex_lock(&mutex2);
                printf("In thread: get mutex2\n");
                //pthread_mutex_lock(&mutex1);
                ret = pthread_mutex_trylock(&mutex1);
                if(ret == 0) {
                        printf("In thread: get mutex1\n");
                        pthread_mutex_unlock(&mutex1);
                }
                pthread_mutex_unlock(&mutex2);
                sleep(rand()%3);
        }


        pthread_exit((void *)1);
}

int main()
{
        pthread_t tid;
        int ret = 0;

        ret = pthread_mutex_init(&mutex1, NULL);
        if(ret != 0) {
                fprintf(stderr,"pthread_mutex_init1 error:%s\n",strerror(ret));
                exit(-1);
        }
        ret = pthread_mutex_init(&mutex2, NULL);
        if(ret != 0) {
                fprintf(stderr,"pthread_mutex_init2 error:%s\n",strerror(ret));
                exit(-1);
        }
        
        ret = pthread_create(&tid, NULL, pthread_func, NULL);
        if(ret != 0) {
                fprintf(stderr,"pthread_create error:%s\n",strerror(ret));
                exit(-1);
        }

        while(1) {
                pthread_mutex_lock(&mutex1);
                printf("In main: get mutex1\n");
                //pthread_mutex_lock(&mutex1);
                ret = pthread_mutex_trylock(&mutex2);
                if(ret == 0) {
                        printf("In main: get mutex2\n");
                        pthread_mutex_unlock(&mutex2);
                }
                pthread_mutex_unlock(&mutex1);
                sleep(rand()%3);
        }

        pthread_mutex_destroy(&mutex1);
        pthread_mutex_destroy(&mutex2);

        return 0;
}
