#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/wait.h>

pthread_mutex_t mutex[5];

void sys_error(char *str)
{
        perror(str);
        exit(-1);
}

int main()
{
        pid_t pid[5];
        pthread_mutexattr_t attr;
        int ret = 0;
        int i = 0;
        int *share_source = NULL;

        /* 设置互斥信号属性，用于进程之间互斥 */
        ret = pthread_mutexattr_init(&attr);
        if(ret != 0) {
                fprintf(stderr,"pthread_mutexattr_init error:%s\n",strerror(ret));
                exit(-1);
        }
        ret = pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
        if(ret != 0) {
                fprintf(stderr,"pthread_mutexattr_setpshared error:%s\n",strerror(ret));
                exit(-1);
        }

        /* 创建互斥信号量 */
        for(int i=0; i<5; i++) {
                ret = pthread_mutex_init(&mutex[i], &attr);
                if(ret != 0) {
                        fprintf(stderr,"pthread_mutex_init error:%s\n",strerror(ret));
                        exit(-1);
                }
        }

        /* 创建匿名映射区 */
        share_source = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
        if(share_source == MAP_FAILED) {
                perror("mmap error");
                exit(-1);
        }

        for(i=0; i<5; i++) {
                pid[i] = fork();
                if(pid[i] == -1)
                        sys_error("fork error");
                else if(pid[i] == 0)
                        break;
        }
        
        if(i < 5) {             //child process
                if(i < 4) {
                        while(1) {
                                pthread_mutex_lock(&mutex[i]);
                                ret = pthread_mutex_trylock(&mutex[i+1]);
                                if(ret == 0) {
                                        (*share_source) ++;
                                        printf("======== %dth fork: share_source=%d ========\n",i,*share_source);
                                        pthread_mutex_unlock(&mutex[i+1]);
                                }
                                pthread_mutex_unlock(&mutex[i]);
                                sleep(1);
                        }
                } else {
                        while(1) {
                                pthread_mutex_lock(&mutex[i+1-5]);
                                ret = pthread_mutex_trylock(&mutex[i]);
                                if(ret == 0) {
                                        (*share_source) ++;
                                        printf("======== %dth fork: share_source=%d ========\n",i,*share_source);
                                        pthread_mutex_unlock(&mutex[i]);
                                }
                                pthread_mutex_unlock(&mutex[i+1-5]);
                                sleep(1);
                        }
                }
        }

        for(int i=0; i<5; i++) {
                wait(NULL);
                pthread_mutex_destroy(&mutex[i]);
        }
        pthread_mutexattr_destroy(&attr);

        return 0;
}
