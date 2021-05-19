#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/mman.h>

void sys_error(char *str)
{
        perror(str);
        exit(-1);
}

int main()
{
        int ret = 0;
        int *p = NULL;
        pid_t pid;
        pthread_mutex_t mutex;
        pthread_mutexattr_t attr;

        pthread_mutexattr_init(&attr);                                   //初始化mutex属性对象
        pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);     //修改属性为进程之间共享
        pthread_mutex_init(&mutex, &attr);                               //初始化mutex

        p = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);   //创建匿名映射区
        if(p == MAP_FAILED)
                sys_error("create mmap error");
        memset(p, 0, sizeof(*p));

        pid = fork();
        if(pid == -1)
                sys_error("fork error");
        else if(pid == 0) {     //child process
                for(int i=0; i<10; i++) {
                        pthread_mutex_lock(&mutex);
                        (*p)++;
                        printf("------ *p = %d ------- (*p)++ \n",*p);
                        pthread_mutex_unlock(&mutex);
                        sleep(1);
                }
        } else {                //parent process
                for(int i=0; i<20; i++) {
                        pthread_mutex_lock(&mutex);
                        (*p)+=2;
                        printf("====== *p = %d ======= (*p)+=2 \n",*p);
                        pthread_mutex_unlock(&mutex);
                        sleep(1);
                }
        }

        return 0;
}
