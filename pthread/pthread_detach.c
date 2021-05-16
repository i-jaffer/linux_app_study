#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

void *pthread_func(void *arg)
{
        int count = 3;

        while(count > 0) {
                printf("=========== thread count: %d ==========\n",count);
                count--;
                sleep(1);
        }
        pthread_exit((void *)1);
}

int main()
{
        pthread_t tid;
        int ret = 0;
        void *retval;

        ret = pthread_create(&tid,NULL,pthread_func,NULL);
        if(ret != 0 ) {
                fprintf(stderr,"pthread_create error:%s\n",strerror(ret));
                exit(-1);
        }

        pthread_detach(tid);            /* thread separate */

        while(1) {
                /* if set pthread_detach, pthread_join will return error */
                ret = pthread_join(tid, (void **)&retval);
                printf("---------ret = %d ----------\n",ret);
                if(ret != 0) {
                        fprintf(stderr,"pthread_join error:%s\n",strerror(ret));
                        exit(-1);
                } else {
                        printf("pthread_join success! retval = %d\n",retval);
                }
                sleep(1);
        }

        return 0;
}
