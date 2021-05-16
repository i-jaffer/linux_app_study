#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

typedef struct {
        int a;
        char c;
        char buf[10];
}retval_typedef;

#if 1   /* 在主线程内创建返回值结构体 由主线程释放地址空间 */

void *pthread_func(void *arg)
{
        retval_typedef *retval = (retval_typedef *)arg;
        retval->a = 99;
        retval->c = 'c';
        strcpy(retval->buf,"zhangsan");

        pthread_exit(NULL);
}

int main()
{
        pthread_t tid;
        int ret = 0;
        retval_typedef *retval;

        retval = malloc(sizeof(retval_typedef));

        ret = pthread_create(&tid,NULL,pthread_func,(void *)retval);
        if(ret != 0) {
                fprintf(stderr,"pthread_create error:%s\n",strerror(ret));
                exit(-1);
        }

        pthread_join(tid, NULL);
        printf("pthread join: ret.a=%d ret.c=%c ret.buf=%s\n",retval->a,retval->c,retval->buf);

        free(retval);
        return 0;
}
#else  /* 在线程内创建返回值结构体 由主线程释放地址空间 */

void *pthread_func(void *arg)
{
        retval_typedef *retval;
        retval = malloc(sizeof(retval_typedef));
        retval->a = 2;
        retval->c = 'a';
        strcpy(retval->buf,"Lili");

        pthread_exit((void *)retval);
}

int main()
{
        pthread_t tid;
        int ret = 0;
        retval_typedef *retval;

        ret = pthread_create(&tid,NULL,pthread_func,NULL);
        if(ret != 0) {
                fprintf(stderr,"pthread_create error:%s\n",strerror(ret));
                exit(-1);
        }

        pthread_join(tid, (void **)&retval);
        printf("pthread join: ret.a=%d ret.c=%c ret.buf=%s\n",retval->a,retval->c,retval->buf);

        free(retval);
        return 0;
}

#endif
