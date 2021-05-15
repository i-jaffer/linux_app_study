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
                printf("child PID = %d,child PID Group = %d\n",getpid(),getpgid(0));
                //printf("child PID = %d,child PID Group = %d\n",getpid(),getpgrp());
                sleep(7);
                printf("Group ID of child is changed to %d\n",getpgid(0));
                printf("After charge, child PID = %d,child PID Group = %d\n",getpid(),getpgid(0));
                exit(0);
        } else {
                sleep(1);
                setpgid(pid,pid);       //set child process group id = child pid
                sleep(7);

                printf("\n");
                printf("parent PID = %d,parent PID Group = %d\n",getpid(),getpgid(0));
                printf("parent's parent PID = %d\n",getppid());
                setpgid(getpid(),getppid());    //set parent process group id = parent's parent id
                printf("Group ID of parent is changed to %d\n",getpgrp());
                printf("After charge, parent PID = %d,parent PID Group = %d\n",getpid(),getpgid(0));
                while(1);
        }

        return 0;
}
