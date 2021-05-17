#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

void *pthread_func(void *arg)
{
        //pthread_cond_timedwait();       /* wait pthread_create return */
        printf("----------- In child thread: RUNING ---------\n");
        pthread_exit((void *)1);
}

int main()
{
        int ret = 0;
        pthread_attr_t attr;
        pthread_t tid;

        /* set thread attribute: detached */
        ret = pthread_attr_init(&attr);
        if(ret != 0) {
                fprintf(stderr,"pthread_attr_init error: %s\n",strerror(ret));
                exit(-1);
        }
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

        ret = pthread_create(&tid, &attr, pthread_func, NULL);
        if(ret != 0) {
                fprintf(stderr,"pthread_create error: %s\n",strerror(ret));
                exit(-1);
        }

        sleep(1);
        /* Beacause child thread is detached,pthread_join will return error number 22! */
        ret = pthread_join(tid, NULL);
        if(ret != 0) {
                fprintf(stderr,"pthread_join error: %s\n",strerror(ret));
                exit(-1);
        }

        pthread_exit((void *)1);
}
