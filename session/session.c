#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
        pid_t pid;
        pid = fork();
        if(pid == -1) {
                perror("fork error");
                exit(-1);
        } else if(pid == 0) {
                sleep(1);
                printf("the child PID = %d\n",getpid());
                printf("the child PGID = %d\n",getpgid(0));
                printf("the child SID = %d\n",getsid(0));

                sleep(1);

                if(setsid() == -1) {             //creat a session
                        perror("setsid error");
                        exit(-1);
                }
                printf("\n============new session===========\n");
                printf("the child PID = %d\n",getpid());
                printf("the child PGID = %d\n",getpgid(0));
                printf("the child SID = %d\n",getsid(0));

        } else {
                printf("the parent PID = %d\n",getpid());
                printf("the parent PGID = %d\n",getpgid(0));
                printf("the parent SID = %d\n",getsid(0));
                printf("\n");
        }
        return 0;
}
