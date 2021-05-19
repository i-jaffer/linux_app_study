#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
        int fd = 0;
        int ret = 0;

        if(argc < 2) {
                printf("please input: ./a.out filename\n");
                exit(-1);
        }

        fd = open(argv[1], O_RDWR);
        if(fd == -1) {
                perror("open error");
                exit(-1);
        }

        struct flock f_lock;

        /* 对整个文件设置 读/写 锁，文件锁同读写锁具有写时共享,读时占有的特点 */
        f_lock.l_type = F_RDLCK;
        f_lock.l_type = F_WRLCK;
        f_lock.l_whence = SEEK_SET;
        f_lock.l_start = 0;
        f_lock.l_len = 0;
        ret = fcntl(fd, F_SETLKW, &f_lock);
        if(ret == -1) {
                perror("open error");
                exit(-1);
        }

        printf("file lock!\n");
        sleep(10);

        f_lock.l_type = F_UNLCK;
        ret = fcntl(fd, F_SETLKW, &f_lock);
        if(ret == -1) {
                perror("open error");
                exit(-1);
        }
        printf("file unlock!\n");

        return 0;
}
