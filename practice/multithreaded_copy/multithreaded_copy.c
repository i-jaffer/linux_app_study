#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int len = 0;
int len_end_thread = 0;
        int fd_src = 0, fd_dest = 0;

void sys_error(char *str)
{
        perror(str);
        exit(-1);
}

void *thread_copy_file(void *arg)
{
        int count = (int)arg;
        int ret = 0;
        char *buf = NULL;

        sleep(count);
        buf = (char *)malloc(len);
        lseek(fd_src,count*len,SEEK_SET);
        ret = read(fd_src,buf,len);
        if(ret == -1)
                sys_error("read error");
        lseek(fd_dest,count*len,SEEK_SET);
        ret = write(fd_dest,buf,len);
        if(ret == -1)
                sys_error("write error");
        printf("-------- %d thread copy end ---------\n",count);

        pthread_exit((void *)1);
}

void *thread_copy_file_end(void *arg)
{
        int count = (int)arg;
        int ret = 0;
        char *buf = NULL;

        sleep(count);
        buf = (char *)malloc(len_end_thread);
        lseek(fd_src,count*len,SEEK_SET);
        ret = read(fd_src,buf,len_end_thread);
        if(ret == -1)
                sys_error("read error");
        lseek(fd_dest,count*len,SEEK_SET);
        ret = write(fd_dest,buf,len_end_thread);
        if(ret == -1)
                sys_error("write error");
        printf("-------- %d thread copy end ---------\n",count);

        pthread_exit((void *)1);
}

int main(int argc, char *argv[])
{
        struct stat statbuf;
        int ret = 0;
        int thread_num = 0;

        /* input check */
        if(argc < 4) {
                printf("please input: ./a.out sourcefile destinationfile threadnum\n");
                exit(-1);
        }

        /* read file message */
        ret = stat(argv[1], &statbuf);
        if(ret == -1)
                sys_error("stat error");
        printf("%s size : %ld\n", argv[1], statbuf.st_size);

        /* open sourcefile and destinationfile */
        fd_src = open(argv[1], O_RDONLY);
        if(fd_src == -1)
                sys_error("open source error");
        fd_dest = open(argv[2], O_RDWR|O_CREAT|O_TRUNC, 0664);
        if(fd_dest == -1)
                sys_error("open destination error");

        /* set destinaion file size */
        if(statbuf.st_size != 0) {
                ret = ftruncate(fd_dest,statbuf.st_size);
                if(ret == -1)
                        sys_error("ftruncate error");
        } else {
                exit(1);
        }

        /* get multithread number */
        thread_num = atoi(argv[3]);
        if(thread_num < 1)
                thread_num = 1;
        if(thread_num > 100)
                thread_num = 100;

        /* calculates the length of bytes per thread copy */
        len = statbuf.st_size/thread_num;
        len_end_thread = statbuf.st_size/thread_num + statbuf.st_size%thread_num;

#if 0   /* set child thread is detach */
        pthread_t tid;
        pthread_attr_t attr;
        ret = pthread_attr_init(&attr);
        if(ret != 0) {
                fprintf(stderr, "pthread_attr_init error : %s",strerror(ret));
                exit(-1);
        }
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        if(ret != 0) {
                fprintf(stderr, "pthread_attr_setdetachstate error : %s",strerror(ret));
                exit(-1);
        }
        for(int i=0; i<thread_num; i++) {
                ret = pthread_create(&tid, &attr, thread_copy_file, NULL);
                if(ret != 0) {
                        fprintf(stderr, "pthread_create error : %s",strerror(ret));
                        exit(-1);
                }
        }
        pthread_attr_destroy(&attr);
#else   /* set child thread is joinable */
        pthread_t tid[thread_num];
        for(int i=0; i<thread_num; i++) {
                if(i < (thread_num-1)) {
                        ret = pthread_create(&tid[i], NULL, thread_copy_file, (void *)i);
                        if(ret != 0) {
                                fprintf(stderr, "pthread_create error : %s",strerror(ret));
                                exit(-1);
                        }
                } else {
                        ret = pthread_create(&tid[i], NULL, thread_copy_file_end, (void *)i);
                        if(ret != 0) {
                                fprintf(stderr, "pthread_create error : %s",strerror(ret));
                                exit(-1);
                        }
                }
        }
        for(int i=0; i<thread_num; i++) {
                pthread_join(tid[i],NULL);
                if(ret != 0) {
                        fprintf(stderr, "pthread_create error : %s",strerror(ret));
                        exit(-1);
                }
        }

        close(fd_src);
        close(fd_dest);
#endif
        return 0;
}
