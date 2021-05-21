#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

int share_source = 0;
pthread_mutex_t mutex[5];

void *thread_eat(void *arg)
{
        int index = (int)arg;
        int ret = 0;

        if(index < 4) {
                while(1) {
                        pthread_mutex_lock(&mutex[index]);
                        ret = pthread_mutex_trylock(&mutex[index+1]);
                        if(ret == 0) {
                                share_source ++;
                                printf("thread %dth: share_source = %d\n",index,share_source);
                                pthread_mutex_unlock(&mutex[index+1]);
                        }
                        pthread_mutex_unlock(&mutex[index]);
                        sleep(1);
                }
        } else {
                while(1) {
                        pthread_mutex_lock(&mutex[index+1-5]);
                        ret = pthread_mutex_trylock(&mutex[index]);
                        if(ret == 0) {
                                share_source ++;
                                printf("thread %dth: share_source = %d\n",index,share_source);
                                pthread_mutex_unlock(&mutex[index]);
                        }
                        pthread_mutex_unlock(&mutex[index+1-5]);
                        sleep(1);
                }

        }

        pthread_exit((void *)1);
}

int main()
{
        pthread_t tid[5];
        int ret = 0;

        for(int i=0; i<5; i++) {
                ret = pthread_mutex_init(&mutex[i], NULL);
                if(ret != 0) {
                        fprintf(stderr, "pthread_mutex_init %dth error: %s\n",i,strerror(ret));
                        exit(-1);
                }
        }

        for(int i=0; i<5; i++) {
                ret = pthread_create(&tid[i],NULL,thread_eat,(void *)i);
                if(ret != 0) {
                        fprintf(stderr, "pthread_create %dth error: %s\n",i,strerror(ret));
                        exit(-1);
                }
        }

        for(int i=0; i<5; i++) {
                ret = pthread_join(tid[i],NULL);
                if(ret != 0) {
                        fprintf(stderr, "pthread_join %dth error: %s\n",i,strerror(ret));
                        exit(-1);
                }
                ret = pthread_mutex_destroy(&mutex[i]);
                if(ret != 0) {
                        fprintf(stderr, "pthread_mutex_destroy %dth error: %s\n",i,strerror(ret));
                        exit(-1);
                }
        }

        return 0;
}
