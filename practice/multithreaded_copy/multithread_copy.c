#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <math.h>

struct copy_param_struct{
        int offset;
        char *p_src_start;
        char *p_dest_start;
        int len;
};

sem_t sem_display;

void sys_error(char *str)
{
        perror(str);
        exit(-1);
}

void progress_display(int target, int *now)
{
        while(*now < target) {
                printf("=");
                fflush(stdout);
                //write(STDOUT_FILENO,"=",sizeof("="));
                (*now)++;
        }
}

/* copy progress display */
void *feedback(void *arg)
{
        int ret = 0;
        int count = 0;
        int thread_num = *(int *)arg;
        int progress_max = 100;         //进度条“=”最大个数
        int progress_now = 0;           //进度条“=”当前个数
        float progress_target = 0;      //进度条“=”当前目标个数
        float progress_eath = progress_max * 1.0 / thread_num;  //进度条每收到一个信号量“=”增长个数

        while(count < thread_num) {
                usleep(1000000/thread_num);
                ret = sem_wait(&sem_display);
                if(ret == -1)
                        sys_error("sem_wait error");
                progress_target += progress_eath;
                progress_display(round(progress_target), &progress_now);
                
                count ++;
        }
        printf("\n");

        pthread_exit((void *)1);
}

/* file copy child thread */
void *file_copy(void *arg)
{
        struct copy_param_struct *copy_param = (struct copy_param_struct*)arg;
        int ret = 0;
#if 0
        printf("src copy start:%p\n",copy_param->p_src_start);
        printf("dest copy start:%p\n",copy_param->p_dest_start);
        printf("copy_param.offset = %d\n",copy_param->offset);
        printf("copy_param.len = %d\n\n",copy_param->len);
#endif
        memcpy(copy_param->p_dest_start, copy_param->p_src_start, copy_param->len);

        /* send semaphore */
        ret = sem_post(&sem_display);
        if(ret == -1)
                sys_error("sem_post error");
        sleep(1);

        pthread_exit((void *)1);
}

int main(int argc, char *argv[])
{
        /* check input */
        if(argc < 4)
        {
                printf("please input: a.out srcfile destfile threadnum\n");
                exit(-1);
        }

        /* open file */
        int fd_src, fd_dest;
        fd_src = open(argv[1], O_RDONLY);
        fd_dest = open(argv[2], O_RDWR|O_CREAT|O_TRUNC, 0664);
        if((fd_src == -1) || (fd_dest == -1))
                sys_error("open error");

        /* check source file size */
        int ret = 0;
        struct stat statbuf;
        ret = fstat(fd_src, &statbuf);
        if(ret == -1)
                sys_error("fstat error");
        printf("src file size:%ld\n",statbuf.st_size);

        /* expand destfile */
        ret = ftruncate(fd_dest, statbuf.st_size);
        if(ret == -1)
                sys_error("ftruncate error");

        /* create semaphore */
        ret = sem_init(&sem_display, 0, 0);
        if(ret == -1)
                sys_error("sem_init error");

        /* create mmap mmapping */
        char *src_p = mmap(NULL, statbuf.st_size, PROT_READ, MAP_PRIVATE, fd_src ,0);
        char *dest_p = mmap(NULL, statbuf.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd_dest ,0);
        if((src_p == MAP_FAILED) || (dest_p == MAP_FAILED))
                sys_error("mmap error");
        printf("src_p  = %p\n",src_p);
        printf("dest_p = %p\n\n",dest_p);

        /* close file */
        close(fd_src);
        close(fd_dest);

        /* get the number of thread , create and calculate each thread paramter */
        int thread_num = atoi(argv[3]);
        if(thread_num < 1)
                thread_num = 1;
        if(thread_num > 100)
                thread_num = 100;

        pthread_t tid_display;
        pthread_create(&tid_display, NULL, feedback, (void*)&thread_num);

        struct copy_param_struct copy_param[thread_num];
        pthread_t *tid = (pthread_t *)malloc(sizeof(pthread_t) * thread_num);
        for(int i=0; i<thread_num; i++) {
                if(i < (thread_num-1)) {
                        copy_param[i].len = statbuf.st_size / thread_num;
                } else {
                        copy_param[i].len = statbuf.st_size / thread_num + (statbuf.st_size % thread_num);
                }
                copy_param[i].offset = i * (statbuf.st_size / thread_num);
                copy_param[i].p_src_start = src_p + copy_param[i].offset;
                copy_param[i].p_dest_start = dest_p + copy_param[i].offset;

                pthread_create(&tid[i], NULL, file_copy, (void *)&copy_param[i]);
        }

        /* receyle child thread */
        for(int i=0; i<thread_num; i++) {
                ret = pthread_join(tid[i], NULL);
                if(ret == -1)
                        sys_error("pthread_join error");
        }
        pthread_join(tid_display, NULL);

        munmap(src_p, statbuf.st_size);
        munmap(dest_p, statbuf.st_size);

        free(tid);
}
