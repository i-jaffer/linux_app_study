#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	int fd = 0;
	fd = open("test.txt",O_WRONLY|O_CREAT,0664);
	if(fd == -1) {
		perror("open error");
		exit(-1);
	}
	dup2(fd,STDOUT_FILENO);		//重定向
	execlp("ps","ps","aux",NULL);
	perror("exec error");
	exit(-1);
//	close(fd);	//注意execlp成功时不会有返回值

	return 0;
}
