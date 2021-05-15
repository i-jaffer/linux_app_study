#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

void Get_systemTime()
{
        time_t timer;
        struct tm *tblock;

        timer = time(NULL);
        tblock = localtime(&timer);
        printf("%d-%d-%d %d:%d:%d\n",tblock->tm_year+1900,tblock->tm_mon+1,tblock->tm_mday,
                                tblock->tm_hour,tblock->tm_min,tblock->tm_sec);
}

void mydaemon(void)
{
        pid_t pid, sid;
        int ret = 0;
        int fd = 0;

        /* create a child process */
        pid = fork();
        if(pid == -1) {
                perror("fork error");
                exit(-1);
        } else if(pid == 0) {
                /* create new session */
                sid = setsid();
                if(sid == -1) {
                        perror("setsid error");
                        exit(-1);
                }

                /* modify the working directory of the process */
                ret = chdir("/home/wl/");
                if(ret == -1) {
                        perror("chdir error");
                        exit(-1);
                }
                
                /* modify the mask value */
                umask(0002);

                /* fd[0]-fd[2] redirect  */
                close(STDIN_FILENO);
                fd = open("/dev/null",O_RDWR);
                ret = dup2(0, STDOUT_FILENO);
                if(ret == -1) {
                        perror("dup2 error");
                        exit(-1);
                }
                ret = dup2(0, STDERR_FILENO);
                if(ret == -1) {
                        perror("dup2 error");
                        exit(-1);
                }
        } else {
                exit(1);
        }
}

int main()
{
        mydaemon();

        int fd = 0;
        fd = open("/home/wl/linux_app_study/daemon/date.txt",O_RDWR|O_CREAT,0664);
        if(fd == -1) {
                perror("open error");
                exit(-1);
        }

        dup2(fd,STDOUT_FILENO);
        close(fd);
        while(1) {
                Get_systemTime();
                fflush(stdout);
                sleep(1);
        }
        return 0;
}
