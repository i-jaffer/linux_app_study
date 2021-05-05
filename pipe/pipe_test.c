#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
	int ret = 0;
	int fd[2] = {0};
	pid_t pid;

	ret = pipe(fd);
	if(ret == -1) {
		perror("pipe error:");
		exit(-1);
	}
	pid = fork();
	if(pid == -1) {
		perror("fork error:");
		exit(-1);
	} else if(pid == 0) {		//子进程 从管道读
		char buf[1024] = {0};
		int size = 0;
		printf("this is child\n");
		close(fd[1]);		//关闭写管道接口
		size = read(fd[0],buf,sizeof(buf));	//read默认会阻塞等待数据
		if(size == -1) {
			perror("read error:");
			exit(-1);
		}
		ret = write(STDOUT_FILENO,buf,size);	//将管道内读取出来的数据打印
		if(ret == -1) {
			perror("write error:");
			exit(-1);
		}
	} else {			//父进程 从管道写
		printf("this is parent\n");
		close(fd[0]);		//关闭读管道接口
		sleep(1);
		ret = write(fd[1],"hell word!\n",strlen("hello word!\n"));
		if(ret == -1) {
			perror("write error:");
			exit(-1);
		}
	}
	return  0;
}
