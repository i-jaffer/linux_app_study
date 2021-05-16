#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int var = 10;

void *thread_func(void *arg)
{
        int i = (int *)arg;
        sleep(i);

        if(i == 1) {
                var = 333;
        } else if(i == 2) {
                var = 777;
        } else if(i == 3) {
                var = 999;
        } else if(i== 4) {
                var = 1222;
        }
        printf("this is %dth thread,var = %d\n",i,var);

        pthread_exit((void *)var);
}

int main()
{
        pthread_t tid[5];
        int ret = 0;
        int *retval = NULL;

        /* The loop create five threads */
        for(int i=0; i<5; i++) {
                ret = pthread_create(&tid[i],NULL,thread_func,(void *)i);
                if(ret != 0) {
                        fprintf(stderr,"pthread_create error:%s\n",strerror(ret));
                        exit(-1);
                }
        }
        
        /* The loop cycle five threads */
        for(int i=0; i<5; i++) {
                ret = pthread_join(tid[i], (void **)&retval);
                if(ret != 0) {
                        fprintf(stderr,"pthread_join error:%s\n",strerror(ret));
                        exit(-1);
                }
                printf("----------cycle %dth thread,retval = %d\n",i,retval);
        }

        return 0;
}
