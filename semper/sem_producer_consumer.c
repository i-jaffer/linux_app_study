#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <pthread.h>
#include "fifo.h"

sem_t sem_product, sem_blank;
fifo_struct_typedef fifo;
pthread_rwlock_t fifo_rwlock;

void sys_error(char *str)
{
        perror(str);
        exit(-1);
}

void *thread_producer(void *arg)
{
        int ret = 0;
        int data;
        while(1) {
                ret = sem_wait(&sem_blank);
                if(ret == -1)
                        sys_error("sem_wait");

                data = rand()%100 + 1;
                printf("+++++++ product %d ++++++++\n",data);
                pthread_rwlock_wrlock(&fifo_rwlock);
                queue_in(&fifo,data);
                pthread_rwlock_unlock(&fifo_rwlock);

                ret = sem_post(&sem_product);
                if(ret == -1)
                        sys_error("sem_wait");

                sleep(rand()%3);
        }
        
        pthread_exit((void *)1);
}

void *thread_consumer(void *arg)
{
        int data = 0;
        int ret = 0;
        while(1) {
                ret = sem_wait(&sem_product);
                if(ret == -1)
                        sys_error("sem_wait");

                pthread_rwlock_rdlock(&fifo_rwlock);
                data = queue_out(&fifo);
                pthread_rwlock_unlock(&fifo_rwlock);
                if(data == -1)
                        printf("fifo out error!\n");
                printf("======== consump %d =======\n",data);

                ret = sem_post(&sem_blank);
                if(ret == -1)
                        sys_error("sem_wait");

                sleep(rand()%3);
        }
        pthread_exit((void *)1);
}

int main()
{
        pthread_t tid_producer, tid_consumer_1, tid_consumer_2;
        int ret = 0;

        ret = pthread_rwlock_init(&fifo_rwlock, NULL);
        if(ret != 0) {
                fprintf(stderr,"pthread_rwlock_init error:%s\n",strerror(ret));
                exit(-1);
        }
        ret = queue_init(&fifo, 5);
        if(ret == -1) {
                printf("queue init error!\n");
                exit(-1);
        }

        ret = sem_init(&sem_product, 0, 0);
        if(ret == -1)
                sys_error("sem_init");
        ret = sem_init(&sem_blank, 0, 5);
        if(ret == -1)
                sys_error("sem_init");

        ret = pthread_create(&tid_producer, NULL, thread_producer, NULL);
        if(ret != 0) {
                fprintf(stderr, "pthread_create producer error: %s\n",strerror(ret));
                exit(-1);
        }
        ret = pthread_create(&tid_consumer_1, NULL, thread_consumer, NULL);
        if(ret != 0) {
                fprintf(stderr, "pthread_create consumer1 error: %s\n",strerror(ret));
                exit(-1);
        }
        ret = pthread_create(&tid_consumer_2, NULL, thread_consumer, NULL);
        if(ret != 0) {
                fprintf(stderr, "pthread_create consumer2 error: %s\n",strerror(ret));
                exit(-1);
        }

        
        pthread_join(tid_producer, NULL);
        pthread_join(tid_consumer_1, NULL);
        pthread_join(tid_consumer_2, NULL);

        sem_destroy(&sem_product);
        sem_destroy(&sem_blank);

        pthread_rwlock_destroy(&fifo_rwlock);

        return 0;
}
