#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

/* @brief: 条件变量与互斥锁构成的生产者与消费者模型 */

struct msg{
        int data;
        struct msg *next;
};

struct msg *head = NULL;
struct msg *msg_p = NULL;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *pthread_producer(void *arg)
{
        int ret = 0;
        while(1) {
                ret = pthread_mutex_lock(&mutex);
                if(ret != 0) {
                        fprintf(stderr,"producer: pthread_mutex_lock error:%s\n",strerror(ret));
                        exit(-1);
                }

                /* tips:msg_p仅为一零时指针变量 */
                msg_p = (struct msg *)malloc(sizeof(struct msg));
                msg_p -> data = (rand()%100)+1;
                msg_p -> next = head;
                head = msg_p;

                ret = pthread_mutex_unlock(&mutex);
                if(ret != 0) {
                        fprintf(stderr,"producer: pthread_mutex_unlock error:%s\n",strerror(ret));
                        exit(-1);
                }
                printf("productor completed: data = %d\n",msg_p->data);
                pthread_cond_signal(&cond);

                sleep(0.1);
        }

        pthread_exit((void *)1);
}

void *pthread_comsumer(void *arg)
{
        int ret = 0;
        while(1) {
                ret = pthread_mutex_lock(&mutex);
                if(ret != 0) {
                        fprintf(stderr,"comsumer: pthread_mutex_lock error:%s\n",strerror(ret));
                        exit(-1);
                }
                while(head == NULL) {
                        pthread_cond_wait(&cond, &mutex);
                }

                printf("----- cumsumer: data = %d ------\n",head->data);
                /* tips:msg_p仅为一零时指针变量 */
                msg_p = head;
                head = msg_p->next;
                free(msg_p);
                msg_p->next = NULL;

                ret = pthread_mutex_unlock(&mutex);
                if(ret != 0) {
                        fprintf(stderr,"comsumer: pthread_mutex_unlock error:%s\n",strerror(ret));
                        exit(-1);
                }

                sleep(0.5);
        }
        pthread_exit((void *)1);
}

int main()
{
        pthread_t tid_producer, tid_consumer;
        int ret = 0;

        ret = pthread_create(&tid_producer, NULL, pthread_producer, NULL);
        if(ret != 0) {
                fprintf(stderr,"pthread_create producer errror : %s\n",strerror(ret));
                exit(-1);
        }
        ret = pthread_create(&tid_consumer, NULL, pthread_comsumer, NULL);
        if(ret != 0) {
                fprintf(stderr,"pthread_create consumer errror : %s\n",strerror(ret));
                exit(-1);
        }

        pthread_join(tid_producer, NULL);
        pthread_join(tid_consumer, NULL);

        return 0;
}
