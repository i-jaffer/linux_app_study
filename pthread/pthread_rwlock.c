#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

/* 创建8个进程，5个进程读共享数据，3个进程写共享数据，使用读写锁进行保护 */

pthread_rwlock_t rwlock;
int count = 0;

void *pthread_read(void *arg)
{
        int serial = (int)arg;
        
        while(1) {
                pthread_rwlock_rdlock(&rwlock);
                printf("========= read %dth thread: %ld, count = %d ==========\n",serial,pthread_self(),count);
                pthread_rwlock_unlock(&rwlock);
                sleep(1);
        }

        pthread_exit((void *)1);
}

void *pthread_write(void *arg)
{
        int serial = (int)arg;
        
        while(1) {
                pthread_rwlock_wrlock(&rwlock);
                printf("========= write %dth thread: %ld, count = %d =========\n",serial,pthread_self(),(++count));
                pthread_rwlock_unlock(&rwlock);
                sleep(1);
        }

        pthread_exit((void *)1);
}

int main()
{
        pthread_t tid[8];
        int ret = 0;
        int i = 0;

        ret = pthread_rwlock_init(&rwlock, NULL);
        if(ret != 0) {
                fprintf(stderr,"pthread_rwlock_init error:%s\n",strerror(ret));
                exit(-1);
        }

        for(i=0; i<5; i++) {
                ret = pthread_create(&tid[i],NULL,pthread_read,(void *)i);
                if(ret != 0) {
                        fprintf(stderr,"pthread_create error:%s\n",strerror(ret));
                        exit(-1);
                }
        }

        for(i=5; i<8; i++) {
                ret = pthread_create(&tid[i],NULL,pthread_write,(void *)i);
                if(ret != 0) {
                        fprintf(stderr,"pthread_create error:%s\n",strerror(ret));
                        exit(-1);
                }
        }

        for(i=0; i<8; i++) {
                ret = pthread_join(tid[i], NULL);
                if(ret != 0) {
                        fprintf(stderr,"pthread_join error:%s\n",strerror(ret));
                        exit(-1);
                }
        }
        pthread_rwlock_destroy(&rwlock);

        return 0;
}
